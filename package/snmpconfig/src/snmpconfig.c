#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>

#include <libcg/cg_general.h>
#include <libcg/cg_ui.h>
#include <libcg/cg_conf.h>

#include <json-c/json.h>

// Configuration
////////////////

#define CONF_FILE			"snmpd"
#define MENU_NAME			"SNMP"

#define SNMP_SEC_SYS		"system"
#define SNMP_SYSCONTACT		"cloudgate@unknown.com"
#define SNMP_SYSNAME		"CloudGate"
#define SNMP_SYSLOCATION	"unknown"
#define SNMP_SYSDESCR		"Option CloudGate"

#define SNMP_SEC_PUBLIC		"public"
#define SNMP_COMMUNITY		"public"

#define SNMP_SEC_SERVER1	"trapsink1"
#define SNMP_SEC_SERVER2	"trapsink2"
#define SNMP_SEC_SERVER3	"trapsink3"
#define SNMP_TRAPSERVER		""
//#define SNMP_TRAPSERVER_COMM	"public"

#define SNMP_TRAP_UPDOWN1	"no"
#define SNMP_TRAP_UPDOWN2	"no"
#define SNMP_TRAP_UPDOWN3	"no"

char *snmp_sysContact = NULL;
char *snmp_sysName = NULL;
char *snmp_sysLocation = NULL;
char *snmp_sysDescr = NULL;
char *snmp_community = NULL;
char *snmp_trapServer1 = NULL;
char *snmp_trapServer2 = NULL;
char *snmp_trapServer3 = NULL;
char *snmp_trapUpDown1 = NULL;
char *snmp_trapUpDown2 = NULL;
char *snmp_trapUpDown3 = NULL;

#define INTERACTIVE

#ifndef INTERACTIVE
#define LOGI(...) {cg_system_log(CG_LOG_INFO, __VA_ARGS__);}
#define LOGD(...) {cg_system_log(CG_LOG_DEBUG, __VA_ARGS__);}
#define LOGE(...) {cg_system_log(CG_LOG_ERR, __VA_ARGS__);}
#define OUTPUT(...) {printf(__VA_ARGS__);}
#else
#define LOGI(...) {printf(__VA_ARGS__);}
#define LOGD(...) {printf(__VA_ARGS__);}
#define LOGE(...) {printf(__VA_ARGS__);}
#define OUTPUT(...) {printf(__VA_ARGS__);}
#endif

// configuration
void get_snmp_conf();
void set_snmp_defaults();

// callbacks
int register_callbacks();

static char *get_snmp_settings_cb(char *json_data, int logged_in, void *context);
static char *set_snmp_settings_cb(char *json_data, int logged_in, void *context);
static char *set_snmp_defaults_cb(char *json_data, int logged_in, void *context);
static char *get_status_cb(char *json_data, int logged_in, void *context);
static char *snmpd_start_cb(char *json_data, int logged_in, void *context);
static char *snmpd_stop_cb(char *json_data, int logged_in, void *context);
static char *snmpd_enable_cb(char *json_data, int logged_in, void *context);
static char *snmpd_disable_cb(char *json_data, int logged_in, void *context);

// user defind signal
void sig_handler(int);


int main (int argc, char *argv[])
{
    cg_status_t ret;
    int status = EXIT_FAILURE;

    signal(SIGUSR1, sig_handler);

    if ((ret = cg_init("snmp")) != CG_STATUS_OK) {
        cg_system_log(CG_LOG_ERR, "cg_init failed with status %d", ret);
        exit(status);
    }
    /////////////////////
    // Read Configuration
    /////////////////////

    // Get snmp configuration
    get_snmp_conf();

    // Register callbacks
    if (!register_callbacks())
        goto error;

    // Register web page
    ret = cg_ui_register_page(MENU_NAME, "snmp/snmp.html");
    if (ret != CG_STATUS_OK) {
        printf("Error registering snmp.html page\n");
        goto error;
    }

    // Bart! An endless loop was eating 66% of the processor!
    for (;;) pause();

    status = EXIT_SUCCESS;
    cg_ui_deregister_page(MENU_NAME);
error:
    cg_deinit();
    exit(status);
}

void sig_handler(int sig)
{
    switch (sig) {
        case SIGUSR1:
            cg_ui_deregister_page(MENU_NAME);
            cg_deinit();
            exit(EXIT_SUCCESS);
        default:
            fprintf(stderr, "Unexpected signal received!\n");
            abort();
    }
}

// Configuration
char *cg_conf_get_default(cg_config_t *ctx, const char *section, const char *option, const char *default_val)
{
    char *retval;
    const char *tmp;

    if (cg_conf_get(ctx, section, option, &tmp) != CG_STATUS_OK) {
        tmp = default_val;
        if (cg_conf_set(ctx, section, option, tmp) != CG_STATUS_OK) {
            LOGE("Error setting default option\n");
        }
    }
    retval = (char *)malloc(strlen(tmp) + 1);
    strcpy(retval, tmp);
    LOGD("Get value %s: %s\n", option, retval);
    return retval;
}

char *cg_conf_set_default(cg_config_t *ctx, const char *section, const char *option, const char *default_val)
{
    char *retval;

    if (cg_conf_set(ctx, section, option, default_val) != CG_STATUS_OK) {
        LOGE("Error setting default option\n");
    }
    retval = strdup(default_val);
    LOGD("Set value %s: %s\n", option, retval);
    return retval;
}

// Get the snmp configuration
// If no configuration available, set defaults
void get_snmp_conf()
{
    cg_config_t *ctx;
    // Open the configuration
    if (cg_conf_open(CONF_FILE, FALSE, &ctx) != CG_STATUS_OK) {
        LOGE("Error opening configuration\n");
    }

    if (snmp_sysContact != NULL) free(snmp_sysContact);
    if (snmp_sysName != NULL) free( snmp_sysName);
    if (snmp_sysLocation != NULL) free(snmp_sysLocation);
    if (snmp_sysDescr != NULL) free(snmp_sysDescr);
    if (snmp_community != NULL) free(snmp_community);

    if (snmp_trapServer1 != NULL) free(snmp_trapServer1);
    if (snmp_trapServer2 != NULL) free(snmp_trapServer2);
    if (snmp_trapServer3 != NULL) free(snmp_trapServer3);

    if (snmp_trapUpDown1 != NULL) free(snmp_trapUpDown1);
    if (snmp_trapUpDown2 != NULL) free(snmp_trapUpDown2);
    if (snmp_trapUpDown3 != NULL) free(snmp_trapUpDown3);

    snmp_sysContact = cg_conf_get_default(ctx, SNMP_SEC_SYS, "sysContact", SNMP_SYSCONTACT);
    snmp_sysName = cg_conf_get_default(ctx, SNMP_SEC_SYS, "sysName", SNMP_SYSNAME);
    snmp_sysLocation = cg_conf_get_default(ctx, SNMP_SEC_SYS, "sysLocation", SNMP_SYSLOCATION);
    snmp_sysDescr = cg_conf_get_default(ctx, SNMP_SEC_SYS, "sysDescr", SNMP_SYSDESCR);

    snmp_community = cg_conf_get_default(ctx, SNMP_SEC_PUBLIC, "community", SNMP_COMMUNITY);

    snmp_trapServer1 = cg_conf_get_default(ctx, SNMP_SEC_SERVER1, "sink2", SNMP_TRAPSERVER);
    snmp_trapServer2 = cg_conf_get_default(ctx, SNMP_SEC_SERVER2, "sink2", SNMP_TRAPSERVER);
    snmp_trapServer3 = cg_conf_get_default(ctx, SNMP_SEC_SERVER3, "sink2", SNMP_TRAPSERVER);

    snmp_trapUpDown1 = cg_conf_get_default(ctx, SNMP_SEC_SERVER1, "linkUpDownNotifications", SNMP_TRAP_UPDOWN1);
    snmp_trapUpDown2 = cg_conf_get_default(ctx, SNMP_SEC_SERVER2, "linkUpDownNotifications", SNMP_TRAP_UPDOWN2);
    snmp_trapUpDown3 = cg_conf_get_default(ctx, SNMP_SEC_SERVER3, "linkUpDownNotifications", SNMP_TRAP_UPDOWN3);

    cg_conf_close(ctx);
}

// Set the snmp configuration to all defaults
void set_snmp_defaults()
{
    cg_config_t *ctx;

    // Open the configuration
    if (cg_conf_open(CONF_FILE, FALSE, &ctx) != CG_STATUS_OK) {
        LOGE("Error opening configuration\n");
    }

    snmp_sysContact = cg_conf_set_default(ctx, SNMP_SEC_SYS, "sysContact", SNMP_SYSCONTACT);
    snmp_sysName = cg_conf_set_default(ctx, SNMP_SEC_SYS, "sysName", SNMP_SYSNAME);
    snmp_sysLocation = cg_conf_set_default(ctx, SNMP_SEC_SYS, "sysLocation", SNMP_SYSLOCATION);
    snmp_sysDescr = cg_conf_set_default(ctx, SNMP_SEC_SYS, "sysDescr", SNMP_SYSDESCR);

    snmp_community = cg_conf_set_default(ctx, SNMP_SEC_PUBLIC, "community", SNMP_COMMUNITY);

    snmp_trapServer1 = cg_conf_set_default(ctx, SNMP_SEC_SERVER1, "sink2", SNMP_TRAPSERVER);
    snmp_trapServer2 = cg_conf_set_default(ctx, SNMP_SEC_SERVER2, "sink2", SNMP_TRAPSERVER);
    snmp_trapServer3 = cg_conf_set_default(ctx, SNMP_SEC_SERVER3, "sink2", SNMP_TRAPSERVER);

    snmp_trapUpDown1 = cg_conf_set_default(ctx, SNMP_SEC_SERVER1, "linkUpDownNotifications", SNMP_TRAP_UPDOWN1);
    snmp_trapUpDown2 = cg_conf_set_default(ctx, SNMP_SEC_SERVER2, "linkUpDownNotifications", SNMP_TRAP_UPDOWN2);
    snmp_trapUpDown3 = cg_conf_set_default(ctx, SNMP_SEC_SERVER3, "linkUpDownNotifications", SNMP_TRAP_UPDOWN3);

    cg_conf_close(ctx);
}


// Register all callbacks
/////////////////////////

int register_callbacks()
{
    cg_status_t cg_status;

    cg_status = cg_ui_register_json_callback("get_snmp_settings_cb", &get_snmp_settings_cb, (void *)"The Context");
    if (cg_status != CG_STATUS_OK) {
        printf("Error register get_snmp_settings_cb callback\n");
        return 0;
    }

    cg_status = cg_ui_register_json_callback("set_snmp_settings_cb", &set_snmp_settings_cb, (void *)"The Context");
    if (cg_status != CG_STATUS_OK) {
        printf("Error register set_snmp_settings_cb callback\n");
        return 0;
    }

    cg_status = cg_ui_register_json_callback("set_snmp_defaults_cb", &set_snmp_defaults_cb, (void *)"The Context");
    if (cg_status != CG_STATUS_OK) {
        printf("Error register set_snmp_defaults_cb callback\n");
        return 0;
    }

    cg_status = cg_ui_register_json_callback("get_status_cb", &get_status_cb, (void *)"The Context");
    if (cg_status != CG_STATUS_OK) {
        printf("Error register get_status_cb callback\n");
        return 0;
    }

    cg_status = cg_ui_register_json_callback("snmpd_start_cb", &snmpd_start_cb, (void *)"The Context");
    if (cg_status != CG_STATUS_OK) {
        printf("Error register snmpd_start_cb callback\n");
        return 0;
    }

    cg_status = cg_ui_register_json_callback("snmpd_stop_cb", &snmpd_stop_cb, (void *)"The Context");
    if (cg_status != CG_STATUS_OK) {
        printf("Error register snmpd_stop_cb callback\n");
        return 0;
    }

    cg_status = cg_ui_register_json_callback("snmpd_enable_cb", &snmpd_enable_cb, (void *)"The Context");
    if (cg_status != CG_STATUS_OK) {
        printf("Error register snmpd_enable_cb callback\n");
        return 0;
    }

    cg_status = cg_ui_register_json_callback("snmpd_disable_cb", &snmpd_disable_cb, (void *)"The Context");
    if (cg_status != CG_STATUS_OK) {
        printf("Error register snmpd_disable_cb callback\n");
        return 0;
    }

    return 1;
}

// snmpd interface functions
////////////////////////////

void snmpd_reload_settings()
{
    system("/etc/init.d/snmpd reload");
}

void snmpd_start()
{
    system("/etc/init.d/snmpd start");
}

void snmpd_restart()
{
    system("/etc/init.d/snmpd restart");
}

void snmpd_stop()
{
    system("/etc/init.d/snmpd stop");
}

void snmpd_enable()
{
    system("/etc/init.d/snmpd enable");
}

void snmpd_disable()
{
    system("/etc/init.d/snmpd disable");
}

int snmpd_isStarted()
{
    if (system("pidof snmpd > /dev/null")) {
        return 0;
    }
    return 1;
}

int snmpd_isEnabled()
{
    if (system("ls /etc/rc.d/S??snmpd &> /dev/null")) {
        return 0;
    }
    return 1;
}

// Callbacks
////////////

// Get the snmp settings
static char *get_snmp_settings_cb(char *json_data, int logged_in, void *context)
{
    char *retval;
    get_snmp_conf();

    asprintf(&retval, "{\"request\":%s,\"context\":\"%s\",\"logged in\":%s,\"isEnabled\":%s,\"sysContact\":\"%s\",\"sysName\":\"%s\",\"sysLocation\":\"%s\",\"sysDescr\":\"%s\",\"community\":\"%s\",\"trapServer1\":\"%s\",\"trapServer2\":\"%s\",\"trapServer3\":\"%s\",\"trapUpDown1\":\"%s\",\"trapUpDown2\":\"%s\",\"trapUpDown3\":\"%s\"}", json_data, (char *)context, logged_in ? "true" : "false",
             snmpd_isEnabled() ? "true" : "false",
             snmp_sysContact,
             snmp_sysName,
             snmp_sysLocation,
             snmp_sysDescr,
             snmp_community,
             snmp_trapServer1,
             snmp_trapServer2,
             snmp_trapServer3,
             snmp_trapUpDown1,
             snmp_trapUpDown2,
             snmp_trapUpDown3
            );

    LOGD("Return data:\n%s\n", retval);

    return retval;
}

// Set the snmp settings to the default values
static char *set_snmp_defaults_cb(char *json_data, int logged_in, void *context)
{
    set_snmp_defaults();

    snmpd_reload_settings();

    return get_snmp_settings_cb(json_data, logged_in, context);
}

// Set the snmp settings
static char *set_snmp_settings_cb(char *json_data, int logged_in, void *context)
{
    cg_config_t *ctx;

    // Open the configuration
    if (cg_conf_open(CONF_FILE, FALSE, &ctx) != CG_STATUS_OK) {
        LOGE("Error opening configuration\n");
    }
    LOGD("Receive data:\n%s\n", json_data);

    json_object *jobj = json_tokener_parse(json_data);
    json_object_object_foreach(jobj, key, val) {
        if (json_object_get_type(val) == json_type_string) {
            if (key && !strcmp((const char *)key, "sysContact")) {
                if (snmp_sysContact != NULL)
                    free(snmp_sysContact);
                snmp_sysContact = strdup(json_object_get_string(val));
                cg_conf_set(ctx, SNMP_SEC_SYS, "sysContact", snmp_sysContact);
            }
            if (key && !strcmp((const char *)key, "sysName")) {
                if (snmp_sysName != NULL)
                    free(snmp_sysName);
                snmp_sysName = strdup(json_object_get_string(val));
                cg_conf_set(ctx, SNMP_SEC_SYS, "sysName", snmp_sysName);
            }
            if (key && !strcmp((const char *)key, "sysLocation")) {
                if (snmp_sysLocation != NULL)
                    free(snmp_sysLocation);
                snmp_sysLocation = strdup(json_object_get_string(val));
                cg_conf_set(ctx, SNMP_SEC_SYS, "sysLocation", snmp_sysLocation);
            }
            if (key && !strcmp((const char *)key, "sysDescr")) {
                if (snmp_sysDescr != NULL)
                    free(snmp_sysDescr);
                snmp_sysDescr = strdup(json_object_get_string(val));
                cg_conf_set(ctx, SNMP_SEC_SYS, "sysDescr", snmp_sysDescr);
            }
            if (key && !strcmp((const char *)key, "community")) {
                if (snmp_community != NULL)
                    free(snmp_community);
                snmp_community = strdup(json_object_get_string(val));
                cg_conf_set(ctx, SNMP_SEC_PUBLIC, "community", snmp_community);
            }
            if (key && !strcmp((const char *)key, "trapServer1")) {
                if (snmp_trapServer1 != NULL)
                    free(snmp_trapServer1);
                snmp_trapServer1 = strdup(json_object_get_string(val));
                cg_conf_set(ctx, SNMP_SEC_SERVER1, "sink2", snmp_trapServer1);
            }
            if (key && !strcmp((const char *)key, "trapServer2")) {
                if (snmp_trapServer2 != NULL)
                    free(snmp_trapServer2);
                snmp_trapServer2 = strdup(json_object_get_string(val));
                cg_conf_set(ctx, SNMP_SEC_SERVER2, "sink2", snmp_trapServer2);
            }
            if (key && !strcmp((const char *)key, "trapServer3")) {
                if (snmp_trapServer3 != NULL)
                    free(snmp_trapServer3);
                snmp_trapServer3 = strdup(json_object_get_string(val));
                cg_conf_set(ctx, SNMP_SEC_SERVER3, "sink2", snmp_trapServer3);
            }
            if (key && !strcmp((const char *)key, "trapUpDown1")) {
                if (snmp_trapUpDown1 != NULL)
                    free(snmp_trapUpDown1);
                snmp_trapUpDown1 = strdup(json_object_get_string(val));
                cg_conf_set(ctx, SNMP_SEC_SERVER1, "linkUpDownNotifications", snmp_trapUpDown1);
            }
            if (key && !strcmp((const char *)key, "trapUpDown2")) {
                if (snmp_trapUpDown2 != NULL)
                    free(snmp_trapUpDown2);
                snmp_trapUpDown2 = strdup(json_object_get_string(val));
                cg_conf_set(ctx, SNMP_SEC_SERVER2, "linkUpDownNotifications", snmp_trapUpDown2);
            }
            if (key && !strcmp((const char *)key, "trapUpDown3")) {
                if (snmp_trapUpDown3 != NULL)
                    free(snmp_trapUpDown3);
                snmp_trapUpDown3 = strdup(json_object_get_string(val));
                cg_conf_set(ctx, SNMP_SEC_SERVER3, "linkUpDownNotifications", snmp_trapUpDown3);
            }

        } else if (json_object_get_type(val) == json_type_boolean) {
            if (key && !strcmp((const char *)key, "isEnabled")) {
                if (json_object_get_boolean(val)) {
                    LOGD("Enabling snmpd\n");
                    snmpd_enable();
                    snmpd_start();
                } else {
                    LOGD("Disabling snmpd\n");
                    snmpd_disable();
                    snmpd_stop();
                }
            }
        }

    }
    cg_conf_close(ctx);

    if (snmpd_isStarted()) {
        snmpd_reload_settings();
    }

    return get_snmp_settings_cb(json_data, logged_in, context);
}


// Get the current status
static char *get_status_cb(char *json_data, int logged_in, void *context)
{
    char *retval;

    asprintf(&retval, "{\"request\":%s,\"context\":\"%s\",\"logged in\":%s,\"running\":%s,\"isEnabled\":%s}",
             json_data,
             (char *)context,
             logged_in ? "true" : "false",
             snmpd_isStarted() ? "true" : "false",
             snmpd_isEnabled() ? "true" : "false"
            );

    LOGD("Return data:\n%s\n", retval);

    return retval;
}

static char *snmpd_start_cb(char *json_data, int logged_in, void *context)
{
    LOGD("Starting snmpd\n");

    snmpd_start();

    return get_status_cb(json_data, logged_in, context);
}

static char *snmpd_stop_cb(char *json_data, int logged_in, void *context)
{
    LOGD("Stopping snmpd\n");

    snmpd_stop();

    return get_status_cb(json_data, logged_in, context);
}

static char *snmpd_enable_cb(char *json_data, int logged_in, void *context)
{
    LOGD("Enabling snmpd\n");

    snmpd_enable();

    return get_status_cb(json_data, logged_in, context);
}

static char *snmpd_disable_cb(char *json_data, int logged_in, void *context)
{
    LOGD("Disabling snmpd\n");

    snmpd_disable();

    return get_status_cb(json_data, logged_in, context);
}
