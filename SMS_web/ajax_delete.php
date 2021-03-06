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
//处理Ajax传入的数据
//`$arr['flag']`为操作是否成功的标志

$arr = $_POST; //若以$.get()方式发送数据，则要改成$_GET.或者干脆:$_REQUEST

$id = trim($arr['MsgId']);		//ID号
$table = trim($arr['table']);	//表名
$arr['flag'] = 0;
//$arr['id'] = $id;
if( $id != '' && $table != '' ) {				//不为空则更新数据库
	require_once('connecting.php');
	
	switch( $table ) {			//根据表名来生成Sql语句
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
	if( $result ) {				//操作成功
		$arr['flag'] = 1;
	}
	mysql_close();
}

//$arr['flag'] = '1';iconv("GB2312","UTF-8",'测试');

$myjson = json_encode($arr);	//编码
echo $myjson;					//回传数据

?>
