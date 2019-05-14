help(){
    echo "Insert device bluetooth name as an argument"
}
 
[ $# -ne 1 ] && {
    hcitool scan 
    help
    exit 0
}
 
[ "$1" = "help" ] && {
    help
    exit 0
}
 
echo "Scanning device $1..."
BT_DEVICE=`hcitool scan |egrep "$1"|awk '{print $1}'`
 
[ `echo $BT_DEVICE|wc -c` -lt 15 ] && {
    echo "Is the device name correct?"
    echo "Exiting..."
    exit 0
}
 
echo "Scanning channel RFCOMM to execute the SPP..."
BT_CHANNEL=`sdptool records $BT_DEVICE|egrep 'Channel'|sed -re 's/Channel: (.*)/\1/'`
 
[ `echo $BT_CHANNEL | wc -c` -lt 1 ] && {
    echo "This device doesn't work with SPP"
    echo "or the support for SPP is not enabled, Exiting..."
    exit 0
}
 
echo "creating virtual serial port /dev/rfcomm0..."
rfcomm bind /dev/rfcomm0 $BT_DEVICE $BT_CHANNEL
 
[ -c /dev/rfcomm0 ] || {
    echo "Wasn't possible to create the virtual port"
    echo "(This script has to be executed as ROOT)"
    echo "Exiting..."
    exit 0
}  
 
echo "Device created successfully"
echo "Done."