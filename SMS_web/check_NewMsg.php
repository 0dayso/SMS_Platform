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
