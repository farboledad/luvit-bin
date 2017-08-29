#Name of the application

#Dallas-sdk application
DALLAS_SDK_APP_SOURCE_PATH=apps/temp

DALLAS_SDK_APP_OBJ= $(DALLAS_SDK_APP_SOURCE_PATH)/temp.o

# Dallas-sdk common/ objects
# Add here files from common directory 
DALLAS_SDK_COMMON_SOURCE_PATH=common

DALLAS_SDK_COMMON_OBJ= $(DALLAS_SDK_COMMON_SOURCE_PATH)/crcutil.o $(DALLAS_SDK_COMMON_SOURCE_PATH)/ioutil.o $(DALLAS_SDK_COMMON_SOURCE_PATH)/owerr.o $(DALLAS_SDK_COMMON_SOURCE_PATH)/findtype.o $(DALLAS_SDK_COMMON_SOURCE_PATH)/temp28.o

# Dallas-sdk lib/general/shared/ objects
# Add here files from lib/general/shared/ directory 
DALLAS_SDK_SHARED_SOURCE_PATH=lib/general/shared

DALLAS_SDK_SHARED_OBJ= $(DALLAS_SDK_SHARED_SOURCE_PATH)/ownet.o $(DALLAS_SDK_SHARED_SOURCE_PATH)/owtran.o

#*************************************************************

OBJS= $(DALLAS_SDK_COMMON_OBJ) $(DALLAS_SDK_SHARED_OBJ) $(DALLAS_SDK_APP_OBJ)

libows.a: $(DALLAS_SDK_COMMON_OBJ) $(DALLAS_SDK_SHARED_OBJ)
	rm -f $@
	$(AR) -rs $@ $(DALLAS_SDK_COMMON_OBJ) $(DALLAS_SDK_SHARED_OBJ)

clean:
	rm -f $(OW_APP_FILE_NAME) $(LIB_OWCORTEX_FILENAME) *.o 

