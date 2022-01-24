url='https://discord.com/api/webhooks/799308529395040286/8oGOMcTph2JvbtjQYRt_UZFytyIvxe-G3AMSx_zxtGfSPu9aWul36B2JaNe-IZXK802a'
nr=`grep -c . filtered`
j=1

while [ $j -le $nr ]
do
i=`head -n 1 filtered`
sed -i -e "1d" filtered
c="VPS-1: "
msg_content=\"$c$i\"
curl -H "Content-Type: application/json" -X POST -d "{\"content\":  $msg_content}" $url
sleep 2
 j=$(( $j + 1 ))
done

