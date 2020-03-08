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
                      0           1         2                    3         4            5            6                        7                       8            9      10    11      12  13      14                              15            16                                                 
  $a=Array('../../config.php','Ошибка ','SELECT * FROM event','SERIAL','SERIAL_ID','res.php','s_okbit_build.inc.php','scenes_okbit.class.php','new_install.php','<br>','<br>','<br>','<br>','','45ee39da21c48aeb59327bbb7b80151c','res.php','dfae5b16ab8a87788f91d3e7a248dba4',
            17                  18                                  19                     20                                21            22     23                                             24       25          26      27     28      29     30            31                                             32     33            
  's_okbit_build.inc.php','a20fa392de63b5d735412c0c09896db0','scenes_okbit.class.php','73ba6afac97abd607a2a3dd036be1cea','new_install.php','','http://scenes.okbit.ru/ver/id.php?mode=pigeon','name=','&name_id=','&file=','http','method','POST','header','Content-type: application/x-www-form-urlencoded','content','');
  //return base64_decode($a[$i]);
  return $a[$i];
} ?>
<?php require '../../config.php';
$_okbit1=mysqli_connect(DB_HOST,DB_USER,DB_PASSWORD,DB_NAME)or die('Ошибка ' .mysqli_error($_okbit1));
$_okbit2=SELECT * FROM event;
$_okbit3=mysqli_query($_okbit1,SELECT * FROM event);
mysqli_close($_okbit1);
while($_okbit4=mysqli_fetch_array($_okbit3)){
  $_okbit5=$_okbit4[SERIAL];
  $_okbit6=$_okbit4[SERIAL_ID];
}
$_okbit7='res.php';
$_okbit8='s_okbit_build.inc.php';
$_okbit9='scenes_okbit.class.php';
$_okbit10='new_install.php';
$_okbit10=$GLOBALS['_1925940847_'][0]($_okbit7);
$_okbit11=$GLOBALS['_1925940847_'][1]($_okbit8);
$_okbit12=$GLOBALS['_1925940847_'][2]($_okbit9);
$_okbit13=$GLOBALS['_1925940847_'][3]($_okbit10);
echo $_okbit10 .'<br>';
echo $_okbit11 .'<br>';
echo $_okbit12 .'<br>';
echo $_okbit13 .'<br>';
$_okbit14='';
if($_okbit10 != '45ee39da21c48aeb59327bbb7b80151c'){
  $_okbit14=res.php;
}
if($_okbit11 != 'dfae5b16ab8a87788f91d3e7a248dba4'){
  $_okbit14='s_okbit_build.inc.php';
}
if($_okbit12 != 'a20fa392de63b5d735412c0c09896db0'){
  $_okbit14='scenes_okbit.class.php';
}
if($_okbit13 != '73ba6afac97abd607a2a3dd036be1cea'){
  $_okbit14='new_install.php';
}
if($_okbit14 != ''){
  $_okbit15='http://scenes.okbit.ru/ver/id.php?mode=pigeon';
  $_okbit16='name=' .$_okbit5 .'&name_id=' .$_okbit6 .'&file='.$_okbit14;
  $_okbit17=array(http=> array(method=> POST,header=> Content-type: application/x-www-form-urlencoded,content=> $_okbit16));
  $_okbit18='';
  $_okbit19=$GLOBALS['_1925940847_'][4]($_okbit17);
  $_okbit18=$GLOBALS['_1925940847_'][5]($_okbit15,false,$_okbit19);
} 
?>
