/**
 * @file IPostAgent.h
 * @brief POSTAGENTʵ���࣬��װPOST�Ĳ������ⲿMODULE����
 * @author cuikehong
 * @version 1.0.0
 * @date 2012-05-08
 */

#ifndef _IPOSTAGENT_H_
#define _IPOSTAGENT_H_

#if defined(__POST_AGENT_DLLEXPORT__)
#   if defined(PostAgent_EXPORTS)
#      define POSTAGENT_API __declspec(dllexport)
#   else 
#      define POSTAGENT_API __declspec(dllimport)
#   endif 
#else 
#   define POSTAGENT_API 
#endif 

namespace platform 
{

namespace founder 
{
    class Packet;
}

namespace postagent 
{

using namespace founder;

typedef unsigned int ChannelID_t;
#define INVALID_CHANNELID ((ChannelID_t)-1)

#define MAX_SERVER_NAME 64


enum ERRORCODE
{
    LOGIN_SUCCESS               = 0,//��¼�ɹ�
    LOGIN_CONNECT_FAIL          = -1,//����POSTʧ��
    LOGIN_POST_DISCONNECT       = -1,//POST�Ͽ�����
    LOGIN_POST_KEY_ERROR        = -2,//KEY��֤����
    CHANNEL_CREATE_ERROR        = -3,//����CHANNEL����
    CHANNEL_DESTROY_ERROR       = -4,//����CHANNEL����
    CHANNEL_CREATE_SELF_REFUSE  = -5,//����ģ��Ҫ������ҡ�����channel�����ҡ��ܾ�
    CHANNEL_CREATE_FULL         = -6,//channel����
    CHANNEL_CREATE_ALREADYCLOSE = -7,//�����Ѿ��ر���
    CHANNEL_CREATE_REFUSE       = -8,//�Է��ܾ�����CHANNEL
    CHANNEL_CREATE_ALREADYBEUSED= -9,//channel ID�Ѿ���ռ��
    CHANNEL_SENDDATA_ERROR      = -10,//���ݴ�����󣬶���ģ���Ѿ�������
};

enum CHANNELMODE
{
    CHANNELMODE_DEFAULT         = 0,//Ĭ��ģʽ����һ��ʱ����û�����ݴ��䣬ֱ�ӹر�
    CHANNELMODE_LONG            = 1,//������ģʽ��һ��ʱ��û�����ݴ��䣬Ҳ���ر�
    CHANNELMODE_MAX
};

enum CHANNEL_STATUS
{
    CHANNEL_UNUSE,		//δʹ��
    CHANNEL_USING,		//��ռ��
    CHANNEL_WAIT_RET,	//�ȴ���֤״̬
    CHANNEL_USED,		//��֤�ɹ�����ʹ��״̬
    CHANNEL_CLOSING,	//�ر�
};


class IPostHandler;
class IPostChannelHandler;

class IPostAgent
{
public:
    //��¼
    virtual bool Login(const char* pszPostIP, unsigned short wPort, const char* szModName = 0, const char* szModKey = 0) = 0;
    //�ǳ�
    virtual bool Logout() = 0;
    //���ڵ���
    virtual void Process() = 0;
    //�ͷ���Դ
    virtual void Release() = 0;
    //����channel
    virtual ChannelID_t CreateChannel(const char* pszDstModName, const char* pszDstModKey) = 0;
    //����channel
    virtual void DestroyChannel(ChannelID_t nChannelID) = 0;
    //����ȫ��channel������nChannelID
    virtual void DestroyAllChannels(ChannelID_t nExcepChannelID = INVALID_CHANNELID) = 0;
    //������Ϣ
    virtual bool SendChannelPacket(ChannelID_t nChannelID, Packet* pPacket) = 0;
    virtual bool SendChannelPacket(ChannelID_t nChannelID, const void* pBuffer, unsigned int dwLength) = 0;
    virtual void SetChannelMode(ChannelID_t nChannelID, CHANNELMODE mode) = 0;

	virtual bool SendUDPPacket(const char *pszdstservername, const void* pdata, unsigned int ui32length) = 0;
    //ע�ᴦ����
    virtual void RegisterPostHandler(IPostHandler* pHandler) = 0;
    virtual void RegisterChannelHandler(ChannelID_t dwChannelID, IPostChannelHandler* pHandler) = 0;
    virtual CHANNEL_STATUS GetChannelStatus(ChannelID_t nChannelID) = 0;    
    virtual const char* GetChannelDestModName(ChannelID_t nChannelID) = 0;
};


/**
 * @brief POST�ص�������
 */
class IPostHandler
{
public:
    
	/**
	 * @brief ��POST���ӽ����ɹ���������֤ͨ��ʱ��Ļص�
	 *
	 * @param errorCode ���ش�����
	 */
    virtual void OnCreate(short errorCode) = 0;
    //POST�Ͽ�����ʱ��Ļص�����
    virtual void OnDestroy() = 0;

    virtual void OnChannelCreate(ChannelID_t dwChannelID, const char* modName) =0;
    virtual void OnChannelDestroy(ChannelID_t dwChannelID) =0;
    virtual void OnChannelError(ChannelID_t dwChannelID, unsigned short errorCode) =0;
    virtual void OnUDPReceive(const char *SourceServer, const void *pData, unsigned int dwLength) = 0;
};


/**
 * @brief channel��Ӧ�ص���
 */
class IPostChannelHandler
{
public:
    virtual void OnChannelRecv(ChannelID_t dwChannelID, const void *pBuffer, unsigned int dwLength) =0;
};

/**
 * @brief ����POSTAGENTΨһ�ӿ�
 *
 * @param dwVersion
 * @param dwMaxChannel
 *
 * @return 
 */
#ifdef __APP_BUILD_COMMIT__
/*extern "C" POSTAGENT_API */IPostAgent* CreatePostAgent(unsigned int dwVersion, unsigned int dwMaxChannel);
#else
extern "C" POSTAGENT_API IPostAgent* CreatePostAgent(unsigned int dwVersion, unsigned int dwMaxChannel);
#endif

//#if defined(__POST_AGENT_DLLEXPORT__)
//inline IPostAgent* MyPostAgentCreate(unsigned int dwVersion, unsigned int dwMaxChannel)
//{
//    typedef IPostAgent* (*POSTAGENT_CREATE_FUNC)(unsigned int dwVersion, unsigned int dwMaxChannel);
//    static const HMODULE hPostAgentDll = LoadLibrary("PostAgent.dll");
//    static const POSTAGENT_CREATE_FUNC funcPostAgentCreate = (POSTAGENT_CREATE_FUNC)GetProcAddress(hPostAgentDll, "CreatePostAgent");
//
//    return funcPostAgentCreate(dwVersion, dwMaxChannel);
//}
//#endif 

} // namespace postagent

} // namespace platform 

#endif
