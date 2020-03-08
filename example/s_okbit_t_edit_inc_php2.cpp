<?php require "../../config.php";
$_SQL_CONNECT=mysqli_connect(DB_HOST,DB_USER,DB_PASSWORD,DB_NAME)or die("Ошибка / Error" .mysqli_error($_SQL_CONNECT));
$_SQL='SELECT * FROM event';
$_SQL_REQUES=mysqli_query($_SQL_CONNECT,$_SQL);
mysqli_close($_SQL_CONNECT);
while($_fetch_array=mysqli_fetch_array($_SQL_REQUES)){
  $serial=$_fetch_array[SERIAL];
  $serial_id=$_fetch_array[SERIAL_ID];
}
$_res_php='res.php';
$_s_okbit_build_inc_php='s_okbit_build.inc.php';
$_scenes_okbit_class_php='scenes_okbit.class.php';
$_new_install_php='new_install.php';
$_md5_new_install_php=md5_file($_res_php);
$_md5_s_okbit_build_inc_php=md5_file($_s_okbit_build_inc_php);
$_md5_scenes_okbit_class_php=md5_file($_scenes_okbit_class_php);
$_md5_new_install_php=md5_file($_new_install_php);
echo $_md5_new_install_php .'<br>';
echo $_md5_s_okbit_build_inc_php .'<br>';
echo $_md5_scenes_okbit_class_php .'<br>';
echo $_md5_new_install_php .'<br>';
$_okbit14='';
if($_new_install_php != '45ee39da21c48aeb59327bbb7b80151c'){
  $_okbit14=res.php;
}
if($_md5_s_okbit_build_inc_php != 'dfae5b16ab8a87788f91d3e7a248dba4'){
  $_okbit14='s_okbit_build.inc.php';
}
if($_md5_scenes_okbit_class_php != 'a20fa392de63b5d735412c0c09896db0'){
  $_okbit14='scenes_okbit.class.php';
}
if($_md5_new_install_php != '73ba6afac97abd607a2a3dd036be1cea'){
  $_okbit14='new_install.php';
}
if($_okbit14 != ''){
  $_okbit15='http://scenes.okbit.ru/ver/id.php?mode=pigeon';
  $_okbit16='name=' .$serial .'&name_id=' .$serial_id .'&file='.$_okbit14;
  $_okbit17=array(http=> array(method=> POST,header=> Content-type: application/x-www-form-urlencoded,content=> $_okbit16));
  $_okbit18='';
  $_okbit19=stream_context_create($_okbit17);
  $_okbit18=file_get_contents($_okbit15,false,$_okbit19);
} 
?>
