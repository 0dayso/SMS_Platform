<?php
//����Ajax���������
//`$arr['flag']`Ϊ�����Ƿ�ɹ��ı�־

$arr = $_POST; //����$.get()��ʽ�������ݣ���Ҫ�ĳ�$_GET.���߸ɴ�:$_REQUEST

$id = trim($arr['MsgId']);		//ID��
$table = trim($arr['table']);	//����
$arr['flag'] = 0;
//$arr['id'] = $id;
if( $id != '' && $table != '' ) {				//��Ϊ����������ݿ�
	require_once('connecting.php');
	
	switch( $table ) {			//���ݱ���������Sql���
		case "receivemessage": $query = "delete from `$table` where `idreceive`='$id'";
			break;
		case "sendmessage": $query = "delete from `$table` where `idsend`='$id'";
			break;
		case "number": $query = "delete from `$table` where `idnum`='$id'";
			break;
		case "replymessage": $query = "delete from `$table` where `id`='$id'";
			break;
		default: $query = "";
			break;
	}//switch
	
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
