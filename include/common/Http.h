/*
*	CGammaHttp.h	httpЭ����
*/

#ifndef _GAMMA_HTTP_H_
#define _GAMMA_HTTP_H_

#include "common/CommonType.h"

namespace Gamma
{
	#define HTTP_REQUEST_HEAD_SIZE 256

	enum EHttpReadState
	{
		eHRS_Ok,
		eHRS_NeedMore,
		eHRS_Error,
		eHRS_Unknow,
	};

	enum EWebSocketID
	{
		eWS_Empty = 0x00,
		eWS_Text = 0x01,
		eWS_Binary = 0x02,
		eWS_Close = 0x08,
		eWS_Ping = 0x09,
		eWS_Pong = 0x0a,
	};

	//  RFC 6455
	//  0                   1                   2                   3
	//  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	// +-+-+-+-+-------+-+-------------+-------------------------------+
	// |F|R|R|R| opcode|M| Payload len |    Extended payload length    |
	// |I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
	// |N|V|V|V|       |S|             |   (if payload len==126/127)   |
	// | |1|2|3|       |K|             |                               |
	// +-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
	// |     Extended payload length continued, if payload len == 127  |
	// + - - - - - - - - - - - - - - - +-------------------------------+
	// |                               |Masking-key, if MASK set to 1  |
	// +-------------------------------+-------------------------------+
	// | Masking-key (continued)       |          Payload Data         |
	// +-------------------------------- - - - - - - - - - - - - - - - +
	// :                     Payload Data continued ...                :
	// + - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
	// |                     Payload Data continued ...                |
	// +---------------------------------------------------------------+

#pragma pack(push,1)
	struct SWebSocketProtocal
	{
		uint8 m_nId : 4;
		uint8 m_nReserved : 3;
		uint8 m_bFinished : 1;
		uint8 m_nLen : 7;
		uint8 m_bMask : 1;
	};
#pragma pack(pop)

	struct SUrlInfo
	{ 
		const char*		szHost; 
		uint32			nHostLen; 
		uint16			nPort; 
		bool			bHttps; 
	};

	class CHttpRecvState
	{
		uint32			m_nHttpLength;
	public:
		CHttpRecvState();
		~CHttpRecvState();

		// ���http�������Ƿ�����
		EHttpReadState	CheckHttpBuffer( char* szBuffer, uint32& nBufferSize );
		uint32			GetDataSize() const;
		void			Reset();
	}; 

	class CHttpRequestState
	{
		uint32			m_nKeepAlive;
		const char*		m_szDataStart;
		uint32			m_nDataLength;
		const char*		m_szPageStart;
		uint32			m_nPageLength;
		const char*		m_szParamStart;
		uint32			m_nParamLength;
		bool			m_bGetMethod;
	public:
		CHttpRequestState();
		~CHttpRequestState();

		// ���http�������Ƿ�����
		EHttpReadState	CheckHttpBuffer( const char* szBuffer, uint32 nBufferSize );
		uint32			GetKeepAlive() const { return m_nKeepAlive; }
		const char*		GetDataStart() const { return m_szDataStart; }
		uint32			GetDataLength() const { return m_nDataLength; }
		const char*		GetPageStart() const { return m_szPageStart; }
		uint32			GetPageLength() const { return m_nPageLength; }
		const char*		GetParamStart() const { return m_szParamStart; }
		uint32			GetParamLength() const { return m_nParamLength; }
		bool			IsGetMethod() const { return m_bGetMethod; }
		void			Reset();
	}; 

	//==============================================================================
	// ��url��ȡszHost�Ͷ˿�
	//==============================================================================
	SUrlInfo GetHostAndPortFromUrl( const char* szUrl );

	//==============================================================================
	// ��װhttprequest
	// nBufferSize�������HTTP_REQUEST_HEAD_SIZE + strlen( szUrl ) + nDataSize
	//==============================================================================
	uint32 MakeHttpRequest( char* szBuffer, uint32 nBufferSize,
		bool bPost, const char* szUrl, const void* pData, uint32 nDataSize);

	//==============================================================================
	// ��װWebSocket request
	// nBufferSize�������HTTP_REQUEST_HEAD_SIZE + strlen( szUrl )
	//==============================================================================
	uint32 MakeWebSocketShakeHand( char* szBuffer, 
		uint32 nBufferSize, uint8 (&aryBinKey)[16], const char* szUrl );
	
	//==============================================================================
	// WebSocket shakehandchec
	// ����ֵ 0��buffer������-1����������szWebSocketKey����������Ϣ > 0:ʹ�õ��Ĵ�С
	//==============================================================================
	uint32 WebSocketShakeHandCheck( const char* pBuffer, size_t nSize, 
		bool bServer, const char*& szWebSocketKey, uint32& nWebSocketKeyLen );

	//==============================================================================
	// ��װWebSocket����˻�Ӧ��Ϣ
	// nBufferSize�������HTTP_REQUEST_HEAD_SIZE
	//==============================================================================
	uint32 MakeWebSocketServerShakeHandResponese( char* szBuffer,
		uint32 nBufferSize, const char* szWebSocketKey, uint32 nWebSocketKeyLen);

	//==============================================================================
	// ��ȡWebSocket Protocol��Ϣ����
	//==============================================================================
	uint64 GetWebSocketProtocolLen( 
		const SWebSocketProtocal* pProtocol, uint64 nSize );

	//==============================================================================
	// ����WebSocket Protocol
	// ����ֵ��ʹ�õ���extra buffer��С
	// char*& pExtraBuffer ���ؽ���������
	// uint64& nSize ���ؽ��������ݴ�С
	//==============================================================================
	uint64 DecodeWebSocketProtocol( 
		const SWebSocketProtocal* pProtocol, char*& pExtraBuffer, uint64& nSize );

	//==============================================================================
	// ����WebSocket Protocol
	// ����ֵ�����ر����Mask��MsgHead.m_bMask = true��Ч
	//==============================================================================
	uint32 EncodeWebSocketProtocol( 
		SWebSocketProtocal& Protocol, char* pExtraBuffer, uint64 nSize );

}

#endif 

