/*
* Copyright (C) 2013 Nivis LLC.
* Email:   opensource@nivis.com
* Website: http://www.nivis.com
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, version 3 of the License.
* 
* Redistribution and use in source and binary forms must retain this
* copyright notice.

* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/

// UdpSocket.cpp: implementation of the CUdpSocket class.
//
//////////////////////////////////////////////////////////////////////


#include "UdpSocket.h"
#include "Common.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUdpSocket::CUdpSocket()
{

}

CUdpSocket::~CUdpSocket()
{

}

bool CUdpSocket::Create(int p_nType, int p_nFamily)
{
	return CWrapSocket::Create( p_nType, p_nFamily );
}


bool CUdpSocket::SendTo(unsigned int p_unTo, int p_nPort, const void *msg, size_t len, int flags /*=0*/ )
{
	sockaddr_in sinTo;

	sinTo.sin_family			= m_nSockFamily;
	sinTo.sin_port				= htons((unsigned short)p_nPort);
	sinTo.sin_addr.s_addr		= p_unTo; //inet_addr(p_szTo);

	int nSend = sendto(	m_socket, msg, len, flags, (struct sockaddr*)&sinTo, sizeof(sinTo) );

	if ( nSend == SOCKET_ERROR )
	{
		LOG_ERR( "CUdpSocket::SendTo(Sock %d To 0x%X Port %u len %u flags %d)", m_socket, p_unTo, p_nPort, len, flags);
		if (errno == EINTR )
		{	return false;
		}

		Close();
		return false;
	}

	return true;
}


bool CUdpSocket::SendTo(const char *p_szTo, int p_nPort, const void *msg, size_t len, int flags /*=0*/ )
{
	return SendTo(inet_addr(p_szTo),p_nPort, msg,len,flags);	
}

//extern int errno;
#include <errno.h>
bool CUdpSocket::RecvFrom(  void *msg, size_t* p_nLen, int flags,
							char *p_szIpFrom , int* p_pPortFrom )
{
	unsigned int unIP;
	
	if (!RecvFrom(msg,p_nLen,flags,&unIP,p_pPortFrom))
	{	return false;
	}
	
	if (p_szIpFrom)
	{	unIP = htonl(unIP);
		strcpy(p_szIpFrom,inet_ntoa(*(struct in_addr*)(&unIP)));
	}
	
	return true;
}

bool CUdpSocket::RecvFrom(  void *msg, size_t* p_nLen, int flags,
						 u_int *p_pnIpFrom , int* p_pPortFrom )
{
	sockaddr_in sinFrom = {0, 0, {0}, {0}};
	socklen_t nAddrLen = sizeof(sinFrom);
	int len = *p_nLen;

	int nSend = recvfrom(	m_socket, msg, len, flags, (struct sockaddr*)&sinFrom, &nAddrLen );

	if (SOCKET_ERROR == nSend)
	{
		LOG_ERR( "CUdpSocket::RecvFrom(Sock %d len %u flags %d)", m_socket, len, flags);
		if (errno == EINTR )
		{	return false;
		}
		Close();
		return false;
	}
	if (nSend == 0)
	{	LOG( "CUdpSocket::RecvFrom() reset" );
		Close();
		return false;
	}

	if (p_pnIpFrom)
	{	*p_pnIpFrom = ntohl(sinFrom.sin_addr.s_addr);
	}
	if (p_pPortFrom)
	{	*p_pPortFrom = ntohs(sinFrom.sin_port);
	}

	*p_nLen = nSend;
	return true;
}


bool CUdpSocket::RecvFrom6(  void *msg, size_t* p_nLen, int flags,  char* p_szIPv6, int* p_pPortFrom )
{
	sockaddr_in6 sinFrom;
	socklen_t nAddrLen = sizeof(sinFrom);

	int len = *p_nLen;

	int nSend = recvfrom (m_socket, msg, len, flags, (struct sockaddr*)&sinFrom, &nAddrLen );

	if (SOCKET_ERROR == nSend)
	{
		LOG_ERR( "CUdpSocket::RecvFrom(Sock %d len %u flags %d)", m_socket, len, flags);
		if (errno == EINTR )
		{	return false;
		}
		Close();
		return false;
	}
	if (nSend == 0)
	{	LOG( "CUdpSocket::RecvFrom() reset" );
		Close();
		return false;
	}

	if (p_szIPv6)
	{	
		if(!inet_ntop(m_nSockFamily, &sinFrom.sin6_addr, p_szIPv6, INET6_ADDRSTRLEN))
		{
			LOG_ERR("CUdpSocket::RecvFrom6: inet_ntop");
			p_szIPv6[0] = 0;
		}
	}
	if (p_pPortFrom)
	{	*p_pPortFrom = ntohs(sinFrom.sin6_port);
	}

	*p_nLen = nSend;
	return true;
}


bool CUdpSocket::SendTo6(const char* p_szTo, int p_nPort, const void *msg, size_t len, int flags /*=0*/ )
{
	struct in6_addr oBinAddress;//sizeof(sockaddr_in6.sin6_addr)];

	if ( !inet_pton(m_nSockFamily, p_szTo, &oBinAddress))
	{
		return false;
	}
	sockaddr_in6 sinTo;

	sinTo.sin6_family			= m_nSockFamily;
	sinTo.sin6_port				= htons((unsigned short)p_nPort);
	sinTo.sin6_addr				= oBinAddress; //inet_addr(p_szTo);

	int nSend = sendto(	m_socket, msg, len, flags, (struct sockaddr*)&sinTo, sizeof(sinTo) );

	if ( nSend == SOCKET_ERROR )
	{
		LOG_ERR( "CUdpSocket::SendTo(Sock %d To %s Port %u len %u flags %d)", m_socket, p_szTo, p_nPort, len, flags);
		if (errno == EINTR )
		{	return false;
		}

		Close();
		return false;
	}

	return true;
}


/**
 * @brief Ask the kernel the message size waiting in queue for m_socket.
 * @retval >=0 The length of the message.
 * @retval -1  Error occured.
 * Does not work on kernels < 2.6.9*/
int CUdpSocket::GetMsgLen( )
{
	char m[4] = {0} ;
	int  n=0;

	sockaddr_in cliaddr ;
	socklen_t len = sizeof(cliaddr );

	n = recvfrom( m_socket, m, 4, MSG_TRUNC|MSG_PEEK, (struct sockaddr *)&cliaddr, &len);

	if ( -1 == n )
		LOG_ERR("CUdpSocket::MsgPeek():[%d]", errno );
	else if ( 0 == n )
		LOG("CUdpSocket::MsgPeek() No message waiting or shutdown() was performed on socket.");
	return n ;
}


bool CUdpSocket::AllowBroadcast(void)
{
	int optval=1; 
	int optlen=sizeof(optval);

	if ( setsockopt( m_socket, SOL_SOCKET, SO_BROADCAST, (char *)&optval,optlen) == SOCKET_ERROR )
	{	LOG_ERR( "CUdpSocket::AllowBroadcast()");
		return false;
	}

	return true;
}

