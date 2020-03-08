s_okbit_t_edit.inc.php
<? $GLOBALS['_1925940847_']=Array(
base64_decode('md5_file'),
base64_decode('md5_file'),
base64_decode('md5_file'),
base64_decode('md5_file'),
base64_decode('stream_context_create'),
base64_decode('file_get_contents')
);?>
<? function translator($i){
  $a=Array('../../config.php','Ошибка ','SELECT * FROM event','SERIAL','SERIAL_ID','res.php','s_okbit_build.inc.php','scenes_okbit.class.php','new_install.php','<br>','<br>','<br>','<br>','','45ee39da21c48aeb59327bbb7b80151c','res.php','dfae5b16ab8a87788f91d3e7a248dba4','s_okbit_build.inc.php','a20fa392de63b5d735412c0c09896db0','scenes_okbit.class.php','73ba6afac97abd607a2a3dd036be1cea','new_install.php','','http://scenes.okbit.ru/ver/id.php?mode=pigeon','name=','&name_id=','&file=','http','method','POST','header','Content-type: application/x-www-form-urlencoded','content','');
  //return base64_decode($a[$i]);
  return $a[$i];
} ?>
<?php require '../../config.php';
$_okbit1=mysqli_connect(DB_HOST,DB_USER,DB_PASSWORD,DB_NAME)or die(translator(1) .mysqli_error($_okbit1));
$_okbit2=translator(2);
$_okbit3=mysqli_query($_okbit1,$_okbit2);
mysqli_close($_okbit1);
while($_okbit4=mysqli_fetch_array($_okbit3)){
  $_okbit5=$_okbit4[translator(3)];
  $_okbit6=$_okbit4[translator(4)];
}
$_okbit7=translator(5);
$_okbit8=translator(6);
$_okbit9=translator(7);
$_okbit10=translator(8);
$_okbit10=$GLOBALS['_1925940847_'][0]($_okbit7);
$_okbit11=$GLOBALS['_1925940847_'][1]($_okbit8);
$_okbit12=$GLOBALS['_1925940847_'][2]($_okbit9);
$_okbit13=$GLOBALS['_1925940847_'][3]($_okbit10);
echo $_okbit10 .translator(9);
echo $_okbit11 .translator(10);
echo $_okbit12 .translator(11);
echo $_okbit13 .translator(12);
$_okbit14=translator(13);
if($_okbit10 != translator(14)){
  $_okbit14=translator(15);
}
if($_okbit11 != translator(16)){
  $_okbit14=translator(17);
}
if($_okbit12 != translator(18)){
  $_okbit14=translator(19);
}
if($_okbit13 != translator(20)){
  $_okbit14=translator(21);
}
if($_okbit14 != translator(22)){
  $_okbit15=translator(23);
  $_okbit16=translator(24) .$_okbit5 .translator(25) .$_okbit6 .translator(26) .$_okbit14;
  $_okbit17=array(translator(27)=> array(translator(28)=> translator(29),translator(30)=> translator(31),translator(32)=> $_okbit16));
  $_okbit18=translator(33);
  $_okbit19=$GLOBALS['_1925940847_'][4]($_okbit17);
  $_okbit18=$GLOBALS['_1925940847_'][5]($_okbit15,false,$_okbit19);
} 
?>
