struct Button{
	float m_fPosX;		//��ʾ������ͶӰ����ϵ(���½�Ϊ����ԭ��)�����꣬
	float m_fPosY;
	float m_fWidth;		//��Ļ���ص�λ
	float m_fHeight;

	int m_bPressed;
	void Render()
	{
		glPushMatrix();
		{
			//������λ��ԭ���cube�ƶ���ʹcube���½�����Ϊm_fPosX,m_fPosY��λ��
			//���뿼��cube��������
			glTranslatef(m_fPosX+m_fWidth/2, m_fPosY+m_fHeight/2, -2.0);		//-2.0ֻ��Ϊ�˰�ť�ɼ�
			if( m_bPressed )
			{
				//double scaleAmt = 10.0 * sin( (double)rand() );
				//double scaleAmt = sin( (double)rand() );
				glScalef(0.9, 0.9, 1.0);
			}
			//cube����λ��ԭ��
			glScalef (m_fWidth, m_fHeight, 5.0);      
			glutSolidCube(1.0);
		}
	}
	bool OnMouseDown(int mousex, int mousey)
	{
		//����λ�ã�mousex��mousey����ϵ��ԭ��λ�����Ͻ�
		//���뽫mousey�任��ԭ��λ�����½ǵ�����ϵ��
		mousey = g_fHeight-mousey;
		if( mousex > m_fPosX && mousex < m_fPosX+m_fWidth &&
			mousey > m_fPosY && mousey < m_fPosY+m_fHeight )
		{
			//printf("button is pressed .... /n");
			m_bPressed = true;
			
			return true;
		}
		return false;
	}
	void OnMouseUp()
	{
		m_bPressed = false;
	}
};