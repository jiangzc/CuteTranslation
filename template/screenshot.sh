#/bin/bash
touch /tmp/ocr; 
f1=`cksum /tmp/ocr`;
gnome-screenshot -a -f /tmp/ocr > /dev/null 2>&1;
f2=`cksum /tmp/ocr`;
if [ "$f1" = "$f2" ];then 
    exit 1;
else 
    exit 0;
fi
