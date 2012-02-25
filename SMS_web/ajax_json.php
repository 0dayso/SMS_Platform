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
$arr = $_POST; //若以$.get()方式发送数据，则要改成$_GET.或者干脆:$_REQUEST

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
//$arr['append'] = '1';iconv("GB2312","UTF-8",'测试');

$myjson = json_encode($arr);
echo $myjson;

?>
