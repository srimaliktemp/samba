#!/usr/bin/env python
#
# common functions for samba-tool python commands
#
# Copyright Andrew Tridgell 2010
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

from samba.dcerpc import nbt
from samba.net import Net


def netcmd_dnsname(lp):
    '''return the full DNS name of our own host. Used as a default
       for hostname when running status queries'''
    return lp.get('netbios name').lower() + "." + lp.get('realm').lower()


def netcmd_finddc(lp, creds):
    '''return domain-name of a writable/ldap-capable DC for the domain.'''
    net = Net(creds=creds, lp=lp)
    realm = lp.get('realm')
    cldap_ret = net.finddc(realm,
                nbt.NBT_SERVER_LDAP | nbt.NBT_SERVER_DS | nbt.NBT_SERVER_WRITABLE)
    return cldap_ret.pdc_dns_name