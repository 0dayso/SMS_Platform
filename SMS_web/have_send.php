<?php
//����Ajax���������
//`$arr['flag']`Ϊ�����Ƿ�ɹ��ı�־

$arr = $_POST; //����$.get()��ʽ�������ݣ���Ҫ�ĳ�$_GET.���߸ɴ�:$_REQUEST

$id = trim($arr['MsgId']);		//ID��
$arr['flag'] = 0;
$arr['have_send'] = 0;			//�Ƿ��͵ı�־��Ĭ��Ϊδ���ͣ�0��
//$arr['id'] = $id;
if( $id != '' ) {				//��Ϊ�����ѯ���ݿ�
	require_once('connecting.php');
	
	$query = "select `flagsend` from `sendmessage` where `idsend`='$id'";
	$result = mysql_query( $query );
	
	if( $result ) {				//�����ɹ�
		$arr['flag'] = 1;
		$info = mysql_fetch_array( $result );
		if( $info['flagsend'] != 0 ) {		//�ѷ���
			$arr['have_send'] = 1;
		}
	}
	mysql_close();
}
//$arr['flag'] = '1';iconv("GB2312","UTF-8",'����');

$myjson = json_encode($arr);	//����
echo $myjson;					//�ش�����

?>
