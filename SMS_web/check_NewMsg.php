<?php
//����Ajax���������
//`$arr['flag']`Ϊ�����Ƿ�ɹ��ı�־

$arr = $_POST; //����$.get()��ʽ�������ݣ���Ҫ�ĳ�$_GET.���߸ɴ�:$_REQUEST

$time = trim($arr['time']);		//ʱ���
$arr['flag'] = 1;
$arr['check_new'] = 0;			//�Ƿ��Ѷ��ı�־��Ĭ��Ϊ�Ѷ���0��

//$time = date("Y-m-d H:i:s", (int)$time);

//$arr['time'] = $time;

if( $time != '' ) {				//��Ϊ�����ѯ���ݿ�
	require_once('connecting.php');
	//��ѯδ����Ϣ
	$query = "SELECT count(*)  FROM `receivemessage`  
					WHERE   UNIX_TIMESTAMP(`timereceive`) > '$time' and `flagreceive`=1";
	$result = mysql_query( $query );
	
	if( $result ) {				//�����ɹ�
		$arr['flag'] = 1;
		list($num_new) = mysql_fetch_row( $result );
		$arr['check_new'] = $num_new;	//�¶�����Ŀ
	}
	mysql_close();
}
//$arr['flag'] = '1';iconv("GB2312","UTF-8",'����');		//����

$myjson = json_encode($arr);	//����
echo $myjson;					//�ش�����

?>
