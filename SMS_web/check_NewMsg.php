<?php
//处理Ajax传入的数据
//`$arr['flag']`为操作是否成功的标志

$arr = $_POST; //若以$.get()方式发送数据，则要改成$_GET.或者干脆:$_REQUEST

$time = trim($arr['time']);		//时间戳
$arr['flag'] = 1;
$arr['check_new'] = 0;			//是否已读的标志，默认为已读（0）

//$time = date("Y-m-d H:i:s", (int)$time);

//$arr['time'] = $time;

if( $time != '' ) {				//不为空则查询数据库
	require_once('connecting.php');
	//查询未读信息
	$query = "SELECT count(*)  FROM `receivemessage`  
					WHERE   UNIX_TIMESTAMP(`timereceive`) > '$time' and `flagreceive`=1";
	$result = mysql_query( $query );
	
	if( $result ) {				//操作成功
		$arr['flag'] = 1;
		list($num_new) = mysql_fetch_row( $result );
		$arr['check_new'] = $num_new;	//新短信数目
	}
	mysql_close();
}
//$arr['flag'] = '1';iconv("GB2312","UTF-8",'测试');		//中文

$myjson = json_encode($arr);	//编码
echo $myjson;					//回传数据

?>
