<?php 
/*
 * A platform based on B/S mode with enough features to be as a SMS server.
 * <SMS_Platform> Copyright (C) <2012>  <Jiangzhe Ren>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * Email : jiangzhe78@gmail.com
 */
?>
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

//分页条数
$page_items = 10;

//得到手机号
//如输入1252015272512386，则返回15272512386
//而输入不合规则的号码则返回false
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

//验证手机号码
//如果不是手机号码则从中提取
//如输入1252015272512386，符合手机号码规则，返回1252015272512386
//而输入不合规则的号码则返回false
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

//根据手机号码得到姓名
//如果数据库中没有则返回号码
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

//根据姓名得到手机号码
//如果数据库中没有则返回空
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
