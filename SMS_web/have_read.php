<?php
//����Ajax���������
//`$arr['flag']`Ϊ�����Ƿ�ɹ��ı�־

$arr = $_POST; //����$.get()��ʽ�������ݣ���Ҫ�ĳ�$_GET.���߸ɴ�:$_REQUEST

$id = trim($arr['MsgId']);		//ID��
$arr['flag'] = 0;
//$arr['id'] = $id;
if( $id != '' ) {				//��Ϊ����������ݿ�
	require_once('connecting.php');
	//�Ѷ����Ϊ0
	$query = "update `receivemessage` set `flagreceive`=0 where `idreceive`='$id'";
	$result = mysql_query( $query );
	
	if( $result ) {				//�����ɹ�
		$arr['flag'] = 1;
	}
	mysql_close();
}
//$arr['flag'] = '1';iconv("GB2312","UTF-8",'����');

$myjson = json_encode($arr);	//����
echo $myjson;					//�ش�����

?>
