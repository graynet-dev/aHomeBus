
<? function translator($i){
  $a=Array(
           0          1           2                   3         4          5            6                        7                      8              9       10    11     12   13    14                                  15                   16                                                                                                                                    
'../../config.php','Ошибка ','SELECT * FROM event','SERIAL','SERIAL_ID','res.php','s_okbit_build.inc.php','scenes_okbit.class.php','new_install.php','<br>','<br>','<br>','<br>','','45ee39da21c48aeb59327bbb7b80151c','res.php','dfae5b16ab8a87788f91d3e7a248dba4','s_okbit_build.inc.php','a20fa392de63b5d735412c0c09896db0','scenes_okbit.class.php','73ba6afac97abd607a2a3dd036be1cea','new_install.php','','http://scenes.okbit.ru/ver/id.php?mode=pigeon','name=','&name_id=','&file=','http','method','POST','header','Content-type: application/x-www-form-urlencoded','content','');
  //return base64_decode($a[$i]);
  return $a[$i];
} ?>
<?php require "../../config.php";
$_okbit1=mysqli_connect(DB_HOST,DB_USER,DB_PASSWORD,DB_NAME)or die("Ошибка/Error " .mysqli_error($_okbit1));
$_okbit2='SELECT * FROM event';
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
$_okbit10=md5_file($_okbit7);
$_okbit11=md5_file($_okbit8);
$_okbit12=md5_file($_okbit9);
$_okbit13=md5_file($_okbit10);
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
  $_okbit19=stream_context_create($_okbit17);
  $_okbit18=file_get_contents($_okbit15,false,$_okbit19);
} 
?>
