VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "CIconEntries"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
'/**************************************************************************
'                 ��Ȩ���� (c), 2004 - 2xxx, �̶�������
'
' ************************   ģ �� ��   :CIconEntries********************
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
Private Const THIS_MODULE_NAME As String = "CIconEntries"


'�ֲ����������漯��
Private m_colCol As Collection

Private m_sFilename As String
'/**********************************�� �� ��  : Filename*********************/
'
'��������:
'
'/**************************************************************************/
Friend Property Let Filename(sFilename As String)
    m_sFilename = sFilename
End Property

Public Property Get Filename() As String
     Filename = m_sFilename
End Property


Friend Function Add() As CIconEntry
    'On Error Resume Next
    Dim oIconEntry As CIconEntry
    
    Set oIconEntry = New CIconEntry
    With oIconEntry
        .Filename = m_sFilename
        .IconName = "#" & CStr(m_colCol.Count + 1)
        '.ItemKey = "K:" & CStr(m_colCol.Count)
        m_colCol.Add oIconEntry, .ItemKey
    End With
    Set Add = oIconEntry
End Function

Public Property Get IconEntry(vKey As Variant) As CIconEntry
    On Error Resume Next
    Set IconEntry = m_colCol(vKey)
End Property



Public Property Get Count() As Long
    Count = m_colCol.Count
End Property


Public Sub Remove(lIconName As Long)
    m_colCol.Remove lIconName
End Sub


Public Property Get NewEnum() As IUnknown
Attribute NewEnum.VB_UserMemId = -4
Attribute NewEnum.VB_MemberFlags = "40"
    '������������ For...Each �﷨ö�ٸü��ϡ�
    Set NewEnum = m_colCol.[_NewEnum]
End Property


Private Sub Class_Initialize()
    '������󴴽�����
    Set m_colCol = New Collection
End Sub


Private Sub Class_Terminate()
    '����ֹ���ƻ�����
    Set m_colCol = Nothing
End Sub

