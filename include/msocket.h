#ifndef _MSOCKET_H_
#define _MSOCKET_H_


//extern "C" void _declspec(dllexport)msocket_send(char* ip,char* _data,int _dataLength,
//	char* pRecv,unsigned short _port,int recvByteSize); 


/*
	����һ������������
	======================================
	pRecvBuf:	������������
	isThread:	�Ƿ񵥶�����һ���߳�

*/
void msocket_server(unsigned short port,int pRecvBuflength,
	CallBackFun parseClient,int isThread);
/*
	��ip��������_data
*/
void msocket_send(char* ip,char* _data,int _dataLength,
	char* pRecv,unsigned short _port,int recvByteSize);

#endif