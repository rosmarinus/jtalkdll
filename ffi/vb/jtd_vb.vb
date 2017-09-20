Imports JTalkDll
Imports System
Imports System.ComponentModel
Imports System.Drawing
Imports System.Windows.Forms
Imports System.Runtime.InteropServices


Public Class Form1
    Inherits Form

    Friend WithEvents ComboBox1 As ComboBox
    Friend WithEvents RichTextBox1 As RichTextBox
    Friend WithEvents Button1 As Button
    Friend WithEvents Button2 As Button
    Dim tts As JTalkTTS

    Public Enum PressMode As Long
        PlayMode
        PauseMode
        ResumeMode
    End Enum
    Dim pMode As PressMode

    'Using OnFinishedCallbackFunc = void(__stdcall *)(bool)
    Delegate Sub OnFinishedCallbackDelegate(paused As Boolean)

    Private Components As System.ComponentModel.IContainer
    Private Sub InitializeComponent()
        Me.tts = New JTalkTTS() With {.VoiceName = "tohoku-f01-neutral"}

        Me.tts.OnFinishedCallback = AddressOf onSpeechFinished

        ' 音響モデルフォルダが変更されたときの処理の登録（使わないけど）
        AddHandler Me.tts.VoiceListChanged, AddressOf sample_VoiceListChanegd

        Me.Size = New Drawing.Size(250, 160)
        Me.MaximumSize = Me.Size
        Me.MinimumSize = Me.Size
        Me.Text = "発声テスト"

        Me.ComboBox1 = New ComboBox()
        Me.ComboBox1.Location = New Drawing.Point(8, 5)
        Me.ComboBox1.Size = New Drawing.Size(218, 20)
        Me.ComboBox1.TabIndex = 0
        Me.ComboBox1.DropDownStyle = ComboBoxStyle.DropDownList
        Dim temp = Me.tts.Voice
        For Each v As VoiceFileInfo In Me.tts.Voices
            Me.ComboBox1.Items.Add(v.Name)
        Next
        Me.ComboBox1.SelectedIndex = 0
        Me.ComboBox1.SelectedItem = temp.Name
        Me.Controls.Add(Me.ComboBox1)

        Me.RichTextBox1 = New RichTextBox()
        Me.RichTextBox1.Location = New Drawing.Point(8, 32)
        Me.RichTextBox1.Size = New Drawing.Size(218, 50)
        Me.RichTextBox1.Text = "何か入力してください"
        Me.RichTextBox1.SelectAll()
        Me.RichTextBox1.TabIndex = 1
        Me.Controls.Add(Me.RichTextBox1)
        Me.ActiveControl = Me.RichTextBox1

        Me.Button1 = New Button()
        Me.Button1.Location = New Drawing.Point(8, 88)
        Me.Button1.Size = New Drawing.Size(98, 25)
        Me.Button1.Text = "発声"
        Me.Button1.TabIndex = 2
        Me.Controls.Add(Me.Button1)

        Me.Button2 = New Button()
        Me.Button2.Location = New Drawing.Point(128, 88)
        Me.Button2.Size = New Drawing.Size(98, 25)
        Me.Button2.Text = "停止"
        Me.Button2.TabIndex = 3
        Me.Controls.Add(Me.Button2)

        Me.pMode = PressMode.PlayMode
    End Sub

    ' 発声完了時の処理
    Private Sub onSpeechFinished(paused As Boolean)
        If Not paused Then
            SyncLock Me
                Me.Button1.Text = "発声"
                Me.pMode = PressMode.PlayMode
            End SyncLock 
        End If
    End Sub

    ' 音響モデルディレクトリが変更されたとき、コンボボックスの項目を再設定する
    Private Sub sample_VoiceListChanegd(sender As Object, e As EventArgs)
        Dim name = Me.tts.Voice.Name
        Me.ComboBox1.Items.Clear
        For Each v As VoiceFileInfo In Me.tts.Voices
            Me.ComboBox1.Items.Add(v.Name)
        Next
        Me.ComboBox1.SelectedItem = name
    End Sub

    Private Sub ComboBox1_SelectedIndexChanged(sender As Object, e As EventArgs) Handles ComboBox1.SelectedIndexChanged
        Me.tts.Voice = Me.tts.Voices(Me.ComboBox1.SelectedIndex)
    End Sub

    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        Select Case Me.pMode
            Case PressMode.PlayMode
                Me.Button1.Text = "一時停止"
                Me.pMode = PressMode.PauseMode
                Me.tts.SpeakAsync(RichTextBox1.Text)
            Case PressMode.PauseMode
                Me.Button1.Text = "再開"
                Me.pMode = PressMode.ResumeMode
                Me.tts.PauseMethod()
            Case PressMode.ResumeMode
                Me.Button1.Text = "一時停止"
                Me.pMode = PressMode.PauseMode
                Me.tts.ResumeMethod()
        End Select
    End Sub

    Private Sub Button2_Click(sender As Object, e As EventArgs) Handles Button2.Click
        Me.tts.stopMethod()
        Me.pMode = PressMode.PlayMode
        Me.Button1.Text = "発声"
    End Sub

    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        InitializeComponent()
    End Sub

    <STAThread()>
    Public Shared Sub Main()
        Application.EnableVisualStyles()
        Application.SetCompatibleTextRenderingDefault(False)
        Application.Run(New Form1())
    End Sub

End Class
