<?php
$arr = $_POST; //����$.get()��ʽ�������ݣ���Ҫ�ĳ�$_GET.���߸ɴ�:$_REQUEST

$id = trim($arr['MsgId']);
$arr['append'] = 0;
$arr['id'] = $id;
if( $id != '' ) {
	require_once('connecting.php');
	
	$query = "update `receivemessage` set `flagreceive`=0 where `idreceive`='$id'";
	$result = mysql_query( $query );
	if( $result ) {
		$arr['append'] = 1;
	}
	mysql_close();
}
//$arr['append'] = '1';iconv("GB2312","UTF-8",'����');

$myjson = json_encode($arr);
echo $myjson;

?>
