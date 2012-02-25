# A platform based on B/S mode with enough features to be as a SMS server.
# <SMS_Platform> Copyright (C) <2012>  <Jiangzhe Ren>
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
# 
# Email	: jiangzhe78@gmail.com
# 

CC=gcc
#CFLAGS=-lpthread -lreadline -lhistory -lcurses -L/usr/lib/mysql/ -lmysqlclient -lz 
CFLAGS=-lpthread  -L/usr/lib/mysql/ -lmysqlclient -DDBG
all=main.o sms.o convert.o send.o receive.o connect.o get_conf.o
#all=main.o connect.o get_conf.o
main: $(all) main.h
	$(CC) -o $@ $^ $(CFLAGS)
%.o: %.c main.h
	$(CC) -c -o $@ $<
clean:
	rm -rf *.o main
