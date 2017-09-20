using JTalkCom;
using System;
using System.IO;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using System.Threading.Tasks;
using System.Threading;
using System.Text.RegularExpressions;
using System.Text;
using System.Windows.Forms.Design;


namespace FormWithButton
{
    public class Form1 : Form
    {
        private JTalkTTS tts = new JTalkTTS { VoiceName = "tohoku-f01-neutral" };
        private ComboBox comboBox1;
        private RichTextBox richTextBox1;
        private Button button1;
        private Button button2;
        private Button button3;
        private CancellationTokenSource cancelToken;
        private TableLayoutPanel tableLayoutPanel1;
        private TableLayoutPanel tableLayoutPanel2;

        private async void buttonSay_Click(object sender, EventArgs ea)
        {
            this.button1.Enabled = false;
            this.richTextBox1.ReadOnly = true;
            this.richTextBox1.Focus();

            this.cancelToken = new CancellationTokenSource();

            var start = this.richTextBox1.SelectionStart;
            var len = 0;

            if (this.richTextBox1.SelectedText != "")
            {
                len = start + this.richTextBox1.SelectionLength;
            }
            else
            {
                len = this.richTextBox1.Text.Length;
                while (true)
                {
                    if (start == len)
                    {
                        start = 0;
                        break;
                    }
                    if (this.richTextBox1.Text[start] == '\n')
                    {
                        start += 1;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            var textStr = this.richTextBox1.Text.Substring(0, len);

            var ret = await Task.Run(() =>
            {
                var pos = start;
                var data = "";
                var length = 0;
                var lastLine = false;
                try
                {
                    while (true)
                    {
                        var substr = textStr.Substring(pos);
                        var m = Regex.Match(substr, @"^([^。\n]*?)(。\n|。|\n)");
                        if (m.Success)
                        {
                            length = m.Length;
                            data = m.Value;
                        }
                        else
                        {
                            lastLine = true;
                            length = substr.Length;
                            data = substr;
                        }

                        data = data.Trim(' ', '\t', '　', '\r', '\n');
                        if (data != "")
                        {
                            Invoke(new Action(() =>
                            {
                                this.richTextBox1.Select(pos, length);
                                this.tts.SpeakAsync(data);
                            }));
                            while (this.tts.IsSpeaking)
                            {
                                cancelToken.Token.ThrowIfCancellationRequested();
                            }
                        }

                        if (lastLine)
                        {
                            break;
                        }

                        if (textStr.Length < pos + length)
                        {
                            break;
                        }
                        pos += length;
                    }
                    return data;
                }
                catch (System.OperationCanceledException)
                {
                    return data;
                }
                finally
                {
                    Invoke(new Action(() => { this.richTextBox1.Select(pos + length, 0); }));
                }
            });

            this.richTextBox1.ReadOnly = false;
            this.button1.Enabled = true;
            this.richTextBox1.Focus();
        }

        private string ReadJapaneseText(string path)
        {
            var encode = "Shift_JIS";
            var sr = new StreamReader(path, Encoding.GetEncoding(encode));
            var text = sr.ReadToEnd();
            sr.Close();
            if (text.Length > 10000)
            {
                throw new Exception("ファイルが大きすぎます。");
            }

            return text;
        }

        public Form1()
        {
            // 音響モデルフォルダが変更されたときの処理の登録
            this.tts.VoiceListChanged += (sender, e) =>
            {
                this.comboBox1.Items.Clear();
                this.comboBox1.Items.AddRange(this.tts.Voices.Select(v => v.Name).ToArray());
                this.comboBox1.SelectedItem = this.tts.Voice.Name;
            };

            this.Size = new Size(300, 200);
            this.MinimumSize = this.Size;
            this.Text = "発声テスト";

            this.comboBox1 = new ComboBox();
            this.comboBox1.Dock = DockStyle.Fill;
            this.comboBox1.TabIndex = 0;
            this.comboBox1.SelectedIndexChanged +=
             (sender, e) => { this.tts.Voice = this.tts.Voices[this.comboBox1.SelectedIndex]; };
            this.comboBox1.DropDownStyle = ComboBoxStyle.DropDownList;
            this.comboBox1.Items.AddRange(this.tts.Voices.Select(v => v.Name).ToArray());
            this.comboBox1.SelectedItem = this.tts.Voice.Name;

            this.richTextBox1 = new RichTextBox();
            this.richTextBox1.Dock = DockStyle.Fill;
            this.richTextBox1.Text = "何か入力してください";
            this.richTextBox1.TabIndex = 1;
            this.richTextBox1.AllowDrop = true;
            this.richTextBox1.DragEnter += (sender, e) =>
            {
                if (e.Data.GetDataPresent(DataFormats.FileDrop))
                {
                    var files = (string[])e.Data.GetData(DataFormats.FileDrop);
                    foreach (var file in files)
                    {
                        if (!System.IO.File.Exists(file))
                        {
                            return;
                        }
                    }
                    e.Effect = DragDropEffects.Copy;
                }
                else
                {
                    e.Effect = DragDropEffects.None;
                }
            };
            this.richTextBox1.DragDrop += (sender, e) =>
            {
                if (e.Data.GetDataPresent(DataFormats.FileDrop))
                {
                    var keep = this.richTextBox1.Text;
                    var files = (string[])e.Data.GetData(DataFormats.FileDrop);
                    this.richTextBox1.Text = "";
                    foreach (var file in files)
                    {
                        var text = this.ReadJapaneseText(file);
                        if (text != "")
                        {
                            this.richTextBox1.Text += text;
                        }
                        else
                        {
                            this.richTextBox1.Text = keep;
                            return;
                        }
                    }
                }
                else
                {
                    e.Effect = DragDropEffects.None;
                }
            };

            this.button1 = new Button();
            this.button1.Dock = DockStyle.Fill;
            this.button1.Text = "発声";
            this.button1.TabIndex = 2;
            this.button1.Click += this.buttonSay_Click;

            this.button2 = new Button();
            this.button2.Dock = DockStyle.Fill;
            this.button2.Text = "停止";
            this.button2.TabIndex = 3;
            this.button2.Click += (sender, e) =>
            {
                if (this.cancelToken != null)
                {
                    this.cancelToken.Cancel();
                }

                if (this.tts.IsSpeaking)
                {
                    this.tts.Stop();
                }
            };

            this.button3 = new Button();
            this.button3.Dock = DockStyle.Fill;
            this.button3.Text = "音声フォルダ変更";
            this.button3.TabIndex = 4;
            this.button3.Click += (sender, e) =>
            {
                var dialog = new FolderBrowserDialog();
                dialog.Description = "音響モデルフォルダ選択";
                dialog.SelectedPath = this.tts.VoiceDir;
                dialog.ShowNewFolderButton = false;
                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    try
                    {
                        this.tts.VoiceDir = dialog.SelectedPath;
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.Message);
                    }
                }
            };


            this.tableLayoutPanel1 = new TableLayoutPanel();
            this.tableLayoutPanel2 = new TableLayoutPanel();

            this.tableLayoutPanel2.ColumnCount = 3;
            this.tableLayoutPanel2.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 30F));
            this.tableLayoutPanel2.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 30F));
            this.tableLayoutPanel2.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 40F));
            this.tableLayoutPanel2.Controls.Add(this.button1, 0, 0);
            this.tableLayoutPanel2.Controls.Add(this.button2, 1, 0);
            this.tableLayoutPanel2.Controls.Add(this.button3, 2, 0);
            this.tableLayoutPanel2.Dock = DockStyle.Fill;
            this.tableLayoutPanel2.Location = new Point(0, 121);
            this.tableLayoutPanel2.Margin = new Padding(0);
            this.tableLayoutPanel2.RowCount = 1;
            this.tableLayoutPanel2.RowStyles.Add(new RowStyle(SizeType.Percent, 100F));
            this.tableLayoutPanel2.Size = new Size(276, 40);
            this.tableLayoutPanel2.TabIndex = 1;

            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.comboBox1, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.richTextBox1, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.tableLayoutPanel2, 0, 2);
            this.tableLayoutPanel1.Dock = DockStyle.Fill;
            this.tableLayoutPanel1.Location = new Point(0, 0);
            this.tableLayoutPanel1.RowCount = 3;
            this.tableLayoutPanel1.RowStyles.Add(new RowStyle(SizeType.Absolute, 30F));
            this.tableLayoutPanel1.RowStyles.Add(new RowStyle(SizeType.Percent, 100F));
            this.tableLayoutPanel1.RowStyles.Add(new RowStyle(SizeType.Absolute, 30F));
            this.tableLayoutPanel1.Size = new Size(276, 161);
            this.tableLayoutPanel1.TabIndex = 0;

            this.Controls.Add(this.tableLayoutPanel1);
            this.ActiveControl = this.richTextBox1;

        }

        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.Run(new Form1());
        }
    }
}
