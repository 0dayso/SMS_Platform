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
