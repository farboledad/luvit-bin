#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <termio.h>
#include <unistd.h>
#include <errno.h>

/* ------ initalization parameters and constants ------*/
char port[20];
struct opcode
{
    int Start;
    int Init;
    int Image_Xfer;
    int Validate;
    int Activate_N_Reset;
    int Reset;
    int Config;
    int Response;
};
const struct opcode SerialOpCode = {1,2,3,4,5,6,9,16};
struct status
{
    int Success;
    int Invalid_State_Err;
    int Not_Supported_Err;
    int Data_Sz_Err;
    int CRC_Err;
    int Op_Failed_Err;
    int Success_Need_Addl_Data;
};
const struct status SerialOpStatus = {1,2,3,4,5,6,7};

/* ------ receive functions ------*/
int rxPacket(int fd, int length, char* buffer)
{
	int rxdBytes = 0;
	int rxdTotalBytes = 0;
	int timeout = 0;
	do
	{
		rxdBytes = read(fd, (void*)buffer+rxdTotalBytes, length);
		rxdTotalBytes += rxdBytes;
		if (rxdBytes == 0)
		{
			timeout++;
			if (timeout == 3) break;
		}
	}while (rxdTotalBytes < length);
	return rxdTotalBytes;
}
int rxOpcodeResp(int fd, int opcode, int status)
{
	char rx[5];
	int rxd;
	rxd = rxPacket(fd,sizeof(rx),rx);
	if (rxd != 5)
	{
	  fprintf(stderr, "invalid response length for opcode %i: %d \r\n", opcode, rxd);
	  return -1;
	}
	else if	((rx[0] != 0xaa) || (rx[2] != SerialOpCode.Response) || (rx[3] != opcode) || (rx[4] != status))
	{
	  fprintf(stderr, "invalid response for opcode %i, %02x:%02x:%02x:%02x:%02x \r\n", opcode, rx[0],rx[1],rx[2],rx[3],rx[4]);
	  return -1;
	}
	return 0;
}

/* ------ transmit functions ------*/
int txPacket(int fd, int opcode, char* pkt, int size)
{
	char* txMessage;
	char* txMsg;
	int pktsize = size + 3; //min size 3 bytes : 0xaa, length and opcode
	int txd = 0; //transmitted bytes
	int i;

	if (pktsize <= 256)
	{
		//txMessage = native message with start frame, length, opcode and image to flash
		txMessage = malloc(pktsize);
		//txMessage = Message where bytes with content 0xaa and 0xab are escaped.
		//This message can max. be twice the size of txMessage
		txMsg = malloc(pktsize*2);

		txMessage[0]=0xaa;
		txMessage[1]=pktsize-1;
		txMessage[2]=opcode;
		if (pkt)
		{
			memcpy(txMessage+3,pkt,size);
		}
		//check message on characters to escape.
		unsigned int j = 0;
		for (i = 0; i < pktsize; i++)
		{
			if (i == 0)
			{
				txMsg[i]=txMessage[i];
			}
			else if (txMessage[i] == 0xaa )
			{
				txMsg[j] = 0xab;
				j++;
				txMsg[j] = 0xac;
			}
			else if (txMessage[i] == 0xab )
			{
				txMsg[j] = 0xab;
				j++;
				txMsg[j] = 0xab;
			}
			else
			{
				txMsg[j] = txMessage[i];
			}
			j++;
		}
		//send the message
		txd  = write(fd,txMsg,j);
		free(txMessage);
		free(txMsg);
		if (j == txd)
			return j;
		else
		{
			fprintf(stderr,"packet not completely send \r\n");
			return -1;
		}
	}
	else
	{
		fprintf(stderr,"invalid tx packet, too big");
		return - 1;
	}
}
/* ------ enable bootloader function------*/
int setBootLoaderActive (int fd)
{
	int len;
	int result;
	char ActivateMsg[4]={0xca,0x9d,0xc6,0xa4};
	char* rxBuffer = NULL;

	len = write(fd,(void*)ActivateMsg, sizeof(ActivateMsg));
	tcdrain(fd);
	if (len != sizeof(ActivateMsg))
	{
		fprintf(stderr, "Error: write activate boot message \r\n");
		return (-1);
	}
	rxBuffer = malloc(37 * sizeof(char));
	result = rxPacket(fd,37,rxBuffer);
	if (result < 37)
	{
		fprintf(stderr, "Error: response activate boot message \r\n");
        free(rxBuffer);
        return (-1);
	}
	else
	{
		fprintf(stdout,"DFU: serial loader ready \r\n");
	}
	free(rxBuffer);
	return 0;
}

/* ------ serial port function ------*/
int openSerial(void)
{
    int fd,attributes;
    struct termios tty;

	fd = open(port, O_RDWR | O_NOCTTY);
	if (fd == -1)
	{
		fprintf(stderr, "Error: Serial port open\r\n");
		return -1;
	}
	else
	{
		//set port configuration to 115200, n , 8 , 1
    	if (tcgetattr(fd, &tty) < 0) {
        	fprintf(stderr, "Error from tcgetattr: %s\n", strerror(errno));
        	return -1;
    	}

    	cfsetospeed(&tty, B115200);
    	cfsetispeed(&tty, B115200);

        tty.c_lflag = 0;            // no signaling chars, no echo,no canonical processing
        tty.c_oflag = 0;            // no remapping, no delays
    	tty.c_cflag |= (CLOCAL | CREAD);    /* ignore modem controls */
    	tty.c_cflag &= ~CSIZE;
    	tty.c_cflag |= CS8;         /* 8-bit characters */
    	tty.c_cflag &= ~PARENB;     /* no parity bit */
    	tty.c_cflag &= ~CSTOPB;     /* only need 1 stop bit */
    	tty.c_cflag &= ~CRTSCTS;    /* no hardware flowcontrol */

        tty.c_cc[VMIN]  = 0;        // read doesn't block
        tty.c_cc[VTIME] = 30;        // 3 seconds read timeout

        if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        	fprintf(stderr,"Error from tcsetattr: %s\n", strerror(errno));
        	return -1;
    	}

    	//set dtr line low to get in bootloader mode
    	ioctl(fd, TIOCMGET, &attributes);
    	//attributes |= (TIOCM_DTR); // set bit, set line LOW
    	attributes &= ~(TIOCM_DTR); // clear bit, set line HIGH
    	ioctl(fd, TIOCMSET, &attributes);
    }
    return fd;
}

/* ------ DFU functions ------*/
int sendDFU(int fd, char* pkt, int length, int opcode, int status)
{
	int result1;
	int result2;
	result1 = txPacket(fd,opcode,pkt,length);
	if (result1 > 0)
		result2 = rxOpcodeResp(fd,opcode,status);
	else
		result2 = -1;

	if (result2 == 0)
		return result1;
	else
		return result2;
}

int xferImageDFU(int fd, char* imagebin, int size_image)
{
	const int IMG_PKT_SIZE = 192;
	char img_pkt[IMG_PKT_SIZE];
	int result = -1;
	int i;
	char* img_pkt_end = NULL;
	int sizeImgEnd = 0;
	int txd_bytes = 0;
	int packets = 0;

	//send image in chunks of 192 bytes
	for (i=0; i < (size_image-IMG_PKT_SIZE); i+=IMG_PKT_SIZE )
	{
		memcpy(img_pkt, imagebin+i, IMG_PKT_SIZE);
		result = sendDFU(fd, img_pkt, IMG_PKT_SIZE, SerialOpCode.Image_Xfer, SerialOpStatus.Success_Need_Addl_Data);
		if (result < 0) break;
		txd_bytes += result;
		packets++;
		if (packets%5 == 0)
			fprintf(stdout, ".");
	}
	//send rest of image
	sizeImgEnd = size_image - (packets*IMG_PKT_SIZE);
	if (sizeImgEnd > 0 && result > 0)
	{
		img_pkt_end = malloc(sizeImgEnd);
		memcpy(img_pkt_end, imagebin+i, sizeImgEnd);
		result = sendDFU(fd, img_pkt_end, sizeImgEnd, SerialOpCode.Image_Xfer, SerialOpStatus.Success);
		free(img_pkt_end);
		fprintf(stdout,"\r\n");
		txd_bytes += result;
	}
	if (result < 0)
		return result;
	else
		return txd_bytes;
}


int doDFU(char* startpkt, char* initpkt, char* image, int imglen)
{
	int result=-1;
	int fd;

	fprintf(stdout, "DFU: open serial port\r\n");


 	fd = openSerial();
 	sleep(2);
 	if (fd > 0)
 	{
 		fprintf (stdout, "DFU: activating serial loader.\r\n");
 		result = setBootLoaderActive(fd);
 		if (result < 0) goto endDFU;
 		fprintf (stdout, "DFU: start\r\n");
 		result = sendDFU(fd, startpkt, 12, SerialOpCode.Start,SerialOpStatus.Success);
 		if (result < 0) goto endDFU;
 		fprintf (stdout, "DFU: init\r\n");
 		result = sendDFU(fd, initpkt, 32, SerialOpCode.Init, SerialOpStatus.Success);
 		if (result < 0) goto endDFU;
 		fprintf (stdout, "DFU: uploading image...\r\n");
 		result = xferImageDFU(fd, image, imglen);
 		if (result < 0) goto endDFU;
 		fprintf (stdout, "DFU: transfered %i bytes.\r\n",result);
 		fprintf (stdout, "DFU: validate\r\n");
 		result = sendDFU(fd, NULL, 0, SerialOpCode.Validate, SerialOpStatus.Success);
 		if (result < 0) goto endDFU;
 		fprintf (stdout, "DFU: activating image\r\n");
 		result = sendDFU(fd, NULL, 0, SerialOpCode.Activate_N_Reset, SerialOpStatus.Success);
 		if (result < 0) goto endDFU;
 		sleep(3);
 		fprintf (stdout, "DFU: Complete!\r\n");
 	}
endDFU:
	close(fd);
 	return result;
}

/* ------ main function ------*/

int main(int argc, char **argv) {

char startbin[12];
char initbin[32];
char *source = NULL;
char *imagebin = NULL;
int result=0;

if (!argv[1] || !argv[2])
{
	fprintf(stdout,"missing parameter(s)\r\n");
	fflush(stdout);
	return -1;
}

memcpy (port, argv[2], sizeof(port));

FILE *fp = fopen(argv[1], "r");
if (fp != NULL) {
    fprintf(stdout, "file open: %s\r\n", argv[1]);
    // Go to the end of the file.
    if (fseek(fp, 0L, SEEK_END) == 0)
    {
        // Get the size of the file.
        unsigned long bufsize = ftell(fp);
    	fprintf(stdout, "file size: %lu \r\n",bufsize);

        // if no image available, do nothing (in file, header = 44byte, image = rest)
        if (bufsize < 45)
        {
        	fprintf(stderr, "Error: no valid file\r\n");
        	result = -1;
        }
        else
        {
        	// Allocate our buffer to that size.
        	source = malloc(sizeof(char) * (bufsize));
        	// Go back to the start of the file.
        	fseek(fp, 0L, SEEK_SET);
        	// Read the entire file into memory.
        	size_t newLen = fread(source, sizeof(char), bufsize, fp);
        	if ( (ferror( fp ) != 0) || (newLen != bufsize))
        	{
            	fprintf(stderr,"Error: file read\r\n");
            	result = -1;
        	}
        	else
        	{
        		unsigned long imagesize = bufsize - 44;
        		imagebin = malloc(imagesize*sizeof(char));
        		if (imagebin)
        		{
        			memcpy(startbin,source,sizeof(startbin));
        			memcpy(initbin,source+12, sizeof(initbin));
        			memcpy(imagebin,source+44,imagesize);
        			result = doDFU(startbin,initbin,imagebin, imagesize);
        			free(imagebin);
        		}
        	}
    		free(source);
    	}
    }
    fclose(fp);
}

return result;
}