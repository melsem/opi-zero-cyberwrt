void open_port()  
 { 
  
     
   fd = open(com_Port, O_RDWR | O_NOCTTY); 
 
   if(fd == -1) 
     {
	printf("Error - NOT open port\n");
	error_log("Error - NOT open port");
     }

   else  
     {  
       struct termios options;  
       tcgetattr(fd, &options);   

       switch(speedBaudRate)
       {
	case 4800:       
          cfsetispeed(&options, B4800); 
          cfsetospeed(&options, B4800); 
        break;

	case 9600:       
          cfsetispeed(&options, B9600); 
          cfsetospeed(&options, B9600); 
        break;

	case 19200:       
          cfsetispeed(&options, B19200); 
          cfsetospeed(&options, B19200); 
        break;

	case 38400:       
          cfsetispeed(&options, B38400); 
          cfsetospeed(&options, B38400); 
        break;

	case 57600:       
          cfsetispeed(&options, B57600); 
          cfsetospeed(&options, B57600); 
        break;

	case 115200:       
          cfsetispeed(&options, B115200); 
          cfsetospeed(&options, B115200); 
        break;

	case 230400:       
          cfsetispeed(&options, B230400); 
          cfsetospeed(&options, B230400); 
        break;

	default: 
          cfsetispeed(&options, B115200); 
          cfsetospeed(&options, B115200);
	  printf("Speed setup default - 115200\n");
        break;
       }

       options.c_cflag |= (CLOCAL | CREAD); 
       options.c_iflag = IGNCR;
       options.c_cflag &= ~PARENB;  
       options.c_cflag &= ~CSTOPB;  
       options.c_cflag &= ~CSIZE;  
       options.c_cflag |= CS8;  
       options.c_cc[VMIN] = 1;  
       options.c_cc[VTIME] = 1;  
       options.c_lflag = ICANON;  
       options.c_oflag = 0;  
       options.c_oflag &= ~OPOST; 
       tcflush(fd, TCIFLUSH);
       tcsetattr(fd, TCSANOW, &options);  
     }
 }

/*
* Linux Serial Port Programming Mini-Howto
* Antonino Iannella, antonino@usa.net
* Version 1.0, March 9th 1997
* http://slackware.cs.utah.edu/pub/slackware/slackware-7.0/docs/Linux-mini-HOWTO/Serial-Port-Programming
*/

/*
12. POSIX control mode flags

 The following table lists the possible control modes for c_cflag.  
   
   Constant     Description
   ________________________
   
   CBAUD        Bit mask for baud rate         
   B0           0 baud (drop DTR)         
   B50          50 baud         
   B75          75 baud         
   B110         110 baud         
   B134         134.5 baud         
   B150         150 baud         
   B200         200 baud         
   B300         300 baud         
   B600         600 baud         
   B1200        1200 baud         
   B1800        1800 baud         
   B2400        2400 baud         
   B4800        4800 baud         
   B9600        9600 baud         
   B19200       19200 baud         
   B38400       38400 baud         
   EXTA         External rate clock         
   EXTB         External rate clock         
   CSIZE        Bit mask for data bits         
   CS5          5 data bits         
   CS6          6 data bits         
   CS7          7 data bits         
   CS8          8 data bits         
   CSTOPB       2 stop bits (1 otherwise)         
   CREAD        Enable receiver         
   PARENB       Enable parity bit         
   PARODD       Use odd parity instead of even         
   HUPCL        Hangup (drop DTR) on last close         
   CLOCAL       Local line - do not change 'owner' of port      
   LOBLK        Block job control output         
   CRTSCTS      Enable hardware flow control (not supported on all platforms)         

 13. POSIX tcsetattr Constants
 
   Constant   Description
   ______________________
   
   TCSANOW    Make changes now without waiting for data to complete
   TCSADRAIN  Wait until everything has been transmitted
   TCSAFLUSH  Flush input and output buffers and make the change

 14. POSIX Local Mode Constants
 
   Constant   Description
   ______________________
   
   ISIG       Enable SIGINTR, SIGSUSP, SIGDSUSP, and SIGQUIT signals
   ICANON     Enable canonical input (else raw)
   XCASE      Map uppercase \lowercase (obselete)
   ECHO       Enable echoing of input characters
   ECHOE      Echo erase character as BS-SP-BS
   ECHOK      Echo NL after kill character
   ECHONL     Echo NL
   NOFLSH     Disable flushing of input buffers after interrupt
              or quit characters
   IEXTEN     Enable extended functions
   ECHOCTL    Echo control characters as ^char and delete as ~?
   ECHOPRT    Echo erased character as character erased
   ECHOKE     BS-SP-BS entire line on line kill
   FLUSHO     Output being flushed
   PENDIN     Retype pending input at next read or input char
   TOSTOP     Send SIGTTOU for background output

 15. POSIX Input Mode Constants

   Constant   Description
   ______________________
   
   INPCK      Enable parity check
   IGNPAR     Ignore parity errors
   PARMRK     Mark parity errors
   ISTRIP     Strip parity bits
   IXON       Enable software flow control (outgoing)
   IXOFF      Enable software flow control (incoming)
   IXANY      Allow any character to start flow again
   IGNBRK     Ignore break condition
   BRKINT     Send a SIGINT when a break condition is detected
   INLCR      Map NL to CR
   IGNCR      Ignore CR
   ICRNL      Map CR to NL
   IUCLC      Map uppercase to lowercase
   IMAXBEL    Echo BEL on input line too long

 16. POSIX Output Mode Constants

   Constant   Description
   ______________________
   
   OPOST      Postprocess output (not set = raw output)
   OLCUC      Map lowercase to uppercase
   ONLCR      Map NL to CR-NL
   OCRNL      Map CR to NL
   NOCR       No CR output at column 0
   ONLRET     NL performs CR function
   OFILL      Use fill characters for delay
   OFDEL      Fill character is DEL
   NLDLY      Mask for delay time needed between lines
   NL0        No delay for NLs
   NL1        Delay further output after newline for 100 milliseconds
   CRDLY      Mask for delay time needed to return carriage to left column
   CR0        No delay for CRs
   CR1        Delay after CRs depending on current column position
   CR2        Delay 100 milliseconds after sending CRs
   CR3        Delay 150 milliseconds after sending CRs
   TABDLY     Mask for delay time needed after TABs
   TAB0       No delay for TABs
   TAB1       Delay after TABs depending on current column position
   TAB2       Delay 100 milliseconds after sending TABs
   TAB3       Expand TAB characters to spaces
   BSDLY      Mask for delay time needed after BSs
   BS0        No delay for BSs
   BS1        Delay 50 milliseconds after sending BSs
   VTDLY      Mask for delay time needed after VTs
   VT0        No delay for VTs
   VT1        Delay 2 seconds after sending VTs
   FFDLY      Mask for delay time needed after FFs
   FF0        No delay for FFs
   FF1        Delay 2 seconds after sending FFs

 17. POSIX Control Character Constants

   Constant   Description          Key
   ______________________________________
   
   VINTR      Interrupt            CTRL-C
   VQUIT      Quit                 CTRL-Z
   VERASE     Erase                Backspace (BS)
   VKILL      Kill-line            CTRL-U
   VEOF       End-of-file          CTRL-D
   VEOL       End-of-line          Carriage return (CR)
   VEOL2      Second end-of-line   Line feed (LF)
   VMIN       Minimum number of characters to read
   VTIME      Time to wait for data (tenths of seconds)
*/
