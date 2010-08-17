VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "CResLib"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
'/**************************************************************************
'                 ��Ȩ���� (c), 2004 - 2xxx, �̶�������
'
' ************************   ģ �� ��   :CResLaLaLa********************
'�� �� ��: V1.0
'��    ��: ��Ե
'��������: 2004��03��07��
'����޸�:
'��������:
'�����б�:
'�޸���ʷ:
'��    ��: 2004��03��07��
'�޸���Ա: ��Ե
'�޸�����: ����
'******************************************************************************/
'
Option Explicit
'��ģ������
Private Const THIS_MODULE_NAME As String = "CResLaLaLa"

'�б��Ƿ��Ѿ���ʼ��
Private m_fIsConstructed As Boolean
Private m_sFilename As String
Private m_oResPackage As CResPackage

'/**********************************�� �� ��  : ResPackage*********************/
'
'��������:��Դ��
'
'/**************************************************************************/
Public Property Get ResPackage() As CResPackage
    If Not m_fIsConstructed Then
        'Set ResPackage = Nothing
        Exit Property
    End If
    
    Set ResPackage = m_oResPackage
End Property



'/**********************************�� �� ��  : IsConstructed*********************/
'
'��������:�б��Ƿ��Ѿ���ʼ��
'
'/**************************************************************************/
Public Property Let IsConstructed(fIsConstructed As Boolean)
    m_fIsConstructed = fIsConstructed
End Property

Public Property Get IsConstructed() As Boolean
     IsConstructed = m_fIsConstructed
End Property


'/**********************************�� �� ��  : Filename*********************/
'
'��������:��Դ�ļ���
'
'/**************************************************************************/
Public Property Let Filename(sFilename As String)
    m_sFilename = sFilename
    
    m_fIsConstructed = False
End Property

Public Property Get Filename() As String
    Filename = m_sFilename
End Property


'/**********************************�� �� ��  : CreateResList*********************/
'��������: ������Դ�б�
'
'��    ��:
'
'�� �� ֵ: Boolean
'/**************************************************************************/
Public Function CreateResList() As Boolean
    On Error GoTo Error_Handler
    
    Set m_oResPackage = Nothing
    
    Set m_oResPackage = New CResPackage
    
    m_oResPackage.Filename = m_sFilename
    

    m_fIsConstructed = CreateResListWithObject(m_oResPackage)
    CreateResList = m_fIsConstructed
    Exit Function
Error_Handler:
    '�Զ��������
    
    '����Ĭ�ϴ���������
    Call DefaultErrorHandler(THIS_MODULE_NAME)
End Function


Private Sub Class_Terminate()
    Set m_oResPackage = Nothing
End Sub