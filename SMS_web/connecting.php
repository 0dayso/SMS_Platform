<?php
# FileName="Connection_php_mysql.htm"
# Type="MYSQL"
# HTTP="true"
$hostname_member = "localhost";
$database_member = "shortmessage";
$username_member = "root";
$password_member = "123321";
$member = mysql_pconnect( $hostname_member, $username_member, $password_member ) or trigger_error(mysql_error(),E_USER_ERROR ); 
mysql_select_db( $database_member );
mysql_query("SET NAMES utf8");

//��ҳ����
$page_items = 10;

//�õ��ֻ���
//������1252015272512386���򷵻�15272512386
//�����벻�Ϲ���ĺ����򷵻�false
function GetPhoneNumber( $number ) {
	$arr = NULL;				//init
	
	if( $number != '' ) {		//not null
		preg_match_all("/13[0-9]{1}[0-9]{8}|15[01289]{1}[0-9]{8}|189[0-9]{8}/", $number, $arr );
	}
	
	if( !is_null( $arr ) ) {	
		return $arr[0][0];
	} else {
		return false;
	}
}

//��֤�ֻ�����
//��������ֻ������������ȡ
//������1252015272512386�������ֻ�������򣬷���1252015272512386
//�����벻�Ϲ���ĺ����򷵻�false
function PhoneNumberDetect( $number ) {
	$ResNumber = '';			//init
	
	if ( !(''==$number || 0 == count($number)) ) {		//not null
		$ResDetect = preg_match("/^13[0-9]{1}[0-9]{8}$|^15[01289]{1}[0-9]{8}$|^189[0-9]{8}$/",$number);
		if ( $ResDetect ) {								
			$ResNumber = $number;
		} else {
			$ResNumber = GetPhoneNumber( $number );
		}
	}//end if
	return $ResNumber;
}

//�����ֻ�����õ�����
//������ݿ���û���򷵻غ���
function Number2Name( $number ) {
	$ResName = $number;
	$ResNumber = PhoneNumberDetect( $number );
	
	if ( $ResNumber ) {
		$query = "select * from `number` where `num` LIKE '$ResNumber'";
		$result = mysql_query( $query );
		if( mysql_num_rows( $result ) > 0 ) {
			$info = mysql_fetch_array( $result );
			if($info != "") {
				$ResName = $info['name'];
			}
		}//else
	}
	return $ResName;
}

//���������õ��ֻ�����
//������ݿ���û���򷵻ؿ�
function Name2Number( $name ) {
	$ResNumber = '';
	
	if ( $name != '' ) {
		$query = "select * from `number` where `name` LIKE '$name'";
		$result = mysql_query( $query );
		if( mysql_num_rows( $result ) > 0 ) {
			$info = mysql_fetch_array( $result );
			if($info != "") {
				$ResNumber = $info['num'];
			}
		}//else
	}
	return $ResNumber;
}
?>
