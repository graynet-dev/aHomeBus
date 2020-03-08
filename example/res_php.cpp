<? $GLOBALS['_scenes_okbit_']=Array('print_r','stream_context_create','file_get_contents','print_r','print_r','print_r','print_r','print_r','print_r'); ?>
<? function scenes_okbit($i){$a=Array("../../config.php","res.int.php","Ошибка ",'name','','mail','','serial','','lic_id','','lic_yes','','<div class="alert alert-danger" role="alert">Заполните все поля!</div>',"http://scenes.okbit.ru/ver/id.php?mode=active",'name=','serial','&bench=','lic_id','&name2=','name','&name3=','mail','http','method','POST','header','Content-type: application/x-www-form-urlencoded','content','serial','lic_id','0','1','0','0','<div class="alert alert-success" role="alert">','</div>','1','<div class="alert alert-danger" role="alert">','</div>','2','<div class="alert alert-danger" role="alert">','</div>','3','<div class="alert alert-danger" role="alert">','</div>','5','<div class="alert alert-danger" role="alert">','</div>','<div class="alert alert-danger" role="alert">','</div>');

return $a[$i];} ?>
<?php require "../../config.php";"res.int.php";
$_okbit1=mysqli_connect($_okbit2,DB_USER,DB_PASSWORD,DB_NAME)or die("Ошибка " .mysqli_error($_okbit1));
$print=($_POST);
if($print['name']==''|| $print[mail]==''|| $print[serial]==''|| $print[lic_id]==''|| $print[lic_yes]==''){
  $GLOBALS['_scenes_okbit_'][0]('<div class="alert alert-danger" role="alert">Заполните все поля!</div>');
}
else{
  $_okbit3="http://scenes.okbit.ru/ver/id.php?mode=active";
  $_okbit4='name=' .$print['serial'] .'&bench=' .$print['lic_id'] .'&name2=' .$print['name'] .'&name3=' .$print['mail'];
  $_okbit5=array('http'=> array('method'=> 'POST','header'=> 'Content-type: application/x-www-form-urlencoded','content'=> $_okbit4));
  $_okbit6=$GLOBALS['_scenes_okbit_'][1]($_okbit5);
  $contents=$GLOBALS['_scenes_okbit_'][2]($_okbit3,false,$_okbit6);
  eval('?' .'>' .$contents .'<' .'?');
  $_okbit7=$print['serial'];
  $_okbit8=$print['lic_id'];
  $_okbit9=$new_key;
  $_okbit10=$new_key;
  if($new_key != '0')
  $_okbit11='1';
  else 
  $_okbit11='0';

  $_okbit12="INSERT INTO `event` (ID,SERIAL,SERIAL_ID,HASH_NEW,HASH_OLD,STATE) VALUES (1,'{$_okbit7}','{$_okbit8}','{$_okbit9}','{$_okbit10}','{$_okbit11}') ON DUPLICATE KEY UPDATE SERIAL='{$_okbit7}', SERIAL_ID='{$_okbit8}', HASH_NEW='{$_okbit9}', HASH_OLD='{$_okbit10}', STATE='{$_okbit11}'";
  mysqli_query($_okbit1,$_okbit12);
  mysqli_close($_okbit1);
  if($new_active == '0')
    $GLOBALS['_scenes_okbit_'][3]('<div class="alert alert-success" role="alert">' .$message1 .'</div>');
  else if($new_active == 1)
    $GLOBALS['_scenes_okbit_'][4]('<div class="alert alert-danger" role="alert">' .$message2 .'</div>');
  else if($new_active == 2)
    $GLOBALS['_scenes_okbit_'][5]('<div class="alert alert-danger" role="alert">' .$message3 .'</div>');
  else if($new_active == 3)
    $GLOBALS['_scenes_okbit_'][6]('<div class="alert alert-danger" role="alert">' .$message4 .'</div>');
  else if($new_active == 5)
    $GLOBALS['_scenes_okbit_'][7]('<div class="alert alert-danger" role="alert">' .$message5 .'</div>');
  else 
    $GLOBALS['_scenes_okbit_'][8]('<div class="alert alert-danger" role="alert">' .$message6 .'</div>');
} ?>