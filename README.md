# client-server-file-and-message-transfer

message
-------

#echoserver:

usage:
	echoserver [options]
	--------------------------------------------------
options:
	-p	Port (Default: 8888)
	-h	Show this help message

#echoclient:

usage:
	echoclient [options]
	--------------------------------------------------
options:
	-p	Port (Default: 8888)
	-m	Message to send to server (Default: "Hello World!")
	-h	Show this help message

_________________________________________________________________________________


file
----

#transferserver:

usage:
	transferserver [options]
	--------------------------------------------------
options:
	-p	Port (Default: 8888)
	-h	Show this help message
	-f	File to serve up (Default: bar.txt)

#transferclient:

usage:
	transferclient [options]
	--------------------------------------------------
options:
	-p	Port (Default: 8888)
	-f	Output filename (Default: foo.txt)
	-h	Show this help message
	