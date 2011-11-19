namespace FlightGear_Communicator
{
    partial class FGCForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.ControlGroupBox = new System.Windows.Forms.GroupBox();
            this.AileronsInput = new System.Windows.Forms.TrackBar();
            this.RudderInput = new System.Windows.Forms.TrackBar();
            this.ControlUpdate = new System.Windows.Forms.TrackBar();
            this.ElevatorInput = new System.Windows.Forms.TrackBar();
            this.label1 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.SimulatorGroupBox = new System.Windows.Forms.GroupBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.RolVOutput = new System.Windows.Forms.TextBox();
            this.label12 = new System.Windows.Forms.Label();
            this.ZAccOutput = new System.Windows.Forms.TextBox();
            this.Z = new System.Windows.Forms.Label();
            this.YawVOutput = new System.Windows.Forms.TextBox();
            this.YAccOutput = new System.Windows.Forms.TextBox();
            this.label11 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.PtcVOutput = new System.Windows.Forms.TextBox();
            this.XAccOutput = new System.Windows.Forms.TextBox();
            this.label10 = new System.Windows.Forms.Label();
            this.testbox2 = new System.Windows.Forms.TextBox();
            this.testbox1 = new System.Windows.Forms.TextBox();
            this.OutPortFreqNumberBox = new System.Windows.Forms.NumericUpDown();
            this.UDPRoutingGroupBox = new System.Windows.Forms.GroupBox();
            this.label4 = new System.Windows.Forms.Label();
            this.InPortFreqNumberBox = new System.Windows.Forms.NumericUpDown();
            this.InPortNumberBox = new System.Windows.Forms.NumericUpDown();
            this.OutPortNumberBox = new System.Windows.Forms.NumericUpDown();
            this.label22 = new System.Windows.Forms.Label();
            this.label21 = new System.Windows.Forms.Label();
            this.label20 = new System.Windows.Forms.Label();
            this.IPAddressTextBox = new System.Windows.Forms.TextBox();
            this.label19 = new System.Windows.Forms.Label();
            this.FGCStartStopButton = new System.Windows.Forms.Button();
            this.HardwareGroupBox = new System.Windows.Forms.GroupBox();
            this.HardwareStartStopButton = new System.Windows.Forms.Button();
            this.SerialPortListBox = new System.Windows.Forms.ListBox();
            this.label5 = new System.Windows.Forms.Label();
            this.SerialPortSearchButton = new System.Windows.Forms.Button();
            this.label6 = new System.Windows.Forms.Label();
            this.BaudRateNumberBox = new System.Windows.Forms.NumericUpDown();
            this.SerialFreqNumberBox = new System.Windows.Forms.NumericUpDown();
            this.label13 = new System.Windows.Forms.Label();
            this.ControlGroupBox.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.AileronsInput)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.RudderInput)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.ControlUpdate)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.ElevatorInput)).BeginInit();
            this.SimulatorGroupBox.SuspendLayout();
            this.groupBox3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.OutPortFreqNumberBox)).BeginInit();
            this.UDPRoutingGroupBox.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.InPortFreqNumberBox)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.InPortNumberBox)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.OutPortNumberBox)).BeginInit();
            this.HardwareGroupBox.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.BaudRateNumberBox)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.SerialFreqNumberBox)).BeginInit();
            this.SuspendLayout();
            // 
            // ControlGroupBox
            // 
            this.ControlGroupBox.Controls.Add(this.AileronsInput);
            this.ControlGroupBox.Controls.Add(this.RudderInput);
            this.ControlGroupBox.Controls.Add(this.ControlUpdate);
            this.ControlGroupBox.Controls.Add(this.ElevatorInput);
            this.ControlGroupBox.Controls.Add(this.label1);
            this.ControlGroupBox.Controls.Add(this.label9);
            this.ControlGroupBox.Controls.Add(this.label3);
            this.ControlGroupBox.Controls.Add(this.label2);
            this.ControlGroupBox.Location = new System.Drawing.Point(12, 12);
            this.ControlGroupBox.Name = "ControlGroupBox";
            this.ControlGroupBox.Size = new System.Drawing.Size(254, 165);
            this.ControlGroupBox.TabIndex = 0;
            this.ControlGroupBox.TabStop = false;
            this.ControlGroupBox.Text = "Control Surface Positions";
            // 
            // AileronsInput
            // 
            this.AileronsInput.Location = new System.Drawing.Point(141, 66);
            this.AileronsInput.Minimum = -10;
            this.AileronsInput.Name = "AileronsInput";
            this.AileronsInput.Size = new System.Drawing.Size(104, 45);
            this.AileronsInput.TabIndex = 14;
            this.AileronsInput.Scroll += new System.EventHandler(this.ControlUpdate_Scroll);
            // 
            // RudderInput
            // 
            this.RudderInput.Location = new System.Drawing.Point(146, 117);
            this.RudderInput.Minimum = -10;
            this.RudderInput.Name = "RudderInput";
            this.RudderInput.Size = new System.Drawing.Size(104, 45);
            this.RudderInput.TabIndex = 13;
            this.RudderInput.Scroll += new System.EventHandler(this.ControlUpdate_Scroll);
            // 
            // ControlUpdate
            // 
            this.ControlUpdate.LargeChange = 1;
            this.ControlUpdate.Location = new System.Drawing.Point(5, 32);
            this.ControlUpdate.Name = "ControlUpdate";
            this.ControlUpdate.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.ControlUpdate.Size = new System.Drawing.Size(45, 104);
            this.ControlUpdate.TabIndex = 12;
            this.ControlUpdate.Scroll += new System.EventHandler(this.ControlUpdate_Scroll);
            // 
            // ElevatorInput
            // 
            this.ElevatorInput.LargeChange = 1;
            this.ElevatorInput.Location = new System.Drawing.Point(56, 32);
            this.ElevatorInput.Minimum = -10;
            this.ElevatorInput.Name = "ElevatorInput";
            this.ElevatorInput.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.ElevatorInput.Size = new System.Drawing.Size(45, 104);
            this.ElevatorInput.TabIndex = 11;
            this.ElevatorInput.Scroll += new System.EventHandler(this.ControlUpdate_Scroll);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(50, 16);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(46, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Elevator";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(2, 16);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(43, 13);
            this.label9.TabIndex = 10;
            this.label9.Text = "Throttle";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(97, 68);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(44, 13);
            this.label3.TabIndex = 5;
            this.label3.Text = "Ailerons";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(97, 123);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(42, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "Rudder";
            // 
            // SimulatorGroupBox
            // 
            this.SimulatorGroupBox.Controls.Add(this.groupBox3);
            this.SimulatorGroupBox.Location = new System.Drawing.Point(9, 183);
            this.SimulatorGroupBox.Name = "SimulatorGroupBox";
            this.SimulatorGroupBox.Size = new System.Drawing.Size(257, 124);
            this.SimulatorGroupBox.TabIndex = 1;
            this.SimulatorGroupBox.TabStop = false;
            this.SimulatorGroupBox.Text = "Simulator Data";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.RolVOutput);
            this.groupBox3.Controls.Add(this.label12);
            this.groupBox3.Controls.Add(this.ZAccOutput);
            this.groupBox3.Controls.Add(this.Z);
            this.groupBox3.Controls.Add(this.YawVOutput);
            this.groupBox3.Controls.Add(this.YAccOutput);
            this.groupBox3.Controls.Add(this.label11);
            this.groupBox3.Controls.Add(this.label8);
            this.groupBox3.Controls.Add(this.label7);
            this.groupBox3.Controls.Add(this.PtcVOutput);
            this.groupBox3.Controls.Add(this.XAccOutput);
            this.groupBox3.Controls.Add(this.label10);
            this.groupBox3.Location = new System.Drawing.Point(8, 17);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(240, 99);
            this.groupBox3.TabIndex = 0;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "IMU";
            // 
            // RolVOutput
            // 
            this.RolVOutput.Location = new System.Drawing.Point(162, 74);
            this.RolVOutput.Name = "RolVOutput";
            this.RolVOutput.Size = new System.Drawing.Size(75, 20);
            this.RolVOutput.TabIndex = 11;
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(163, 58);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(51, 13);
            this.label12.TabIndex = 8;
            this.label12.Text = "Roll Rate";
            // 
            // ZAccOutput
            // 
            this.ZAccOutput.Location = new System.Drawing.Point(162, 32);
            this.ZAccOutput.Name = "ZAccOutput";
            this.ZAccOutput.Size = new System.Drawing.Size(72, 20);
            this.ZAccOutput.TabIndex = 5;
            // 
            // Z
            // 
            this.Z.AutoSize = true;
            this.Z.Location = new System.Drawing.Point(159, 16);
            this.Z.Name = "Z";
            this.Z.Size = new System.Drawing.Size(41, 13);
            this.Z.TabIndex = 4;
            this.Z.Text = "ZAccel";
            // 
            // YawVOutput
            // 
            this.YawVOutput.Location = new System.Drawing.Point(86, 74);
            this.YawVOutput.Name = "YawVOutput";
            this.YawVOutput.Size = new System.Drawing.Size(70, 20);
            this.YawVOutput.TabIndex = 10;
            // 
            // YAccOutput
            // 
            this.YAccOutput.Location = new System.Drawing.Point(83, 32);
            this.YAccOutput.Name = "YAccOutput";
            this.YAccOutput.Size = new System.Drawing.Size(73, 20);
            this.YAccOutput.TabIndex = 3;
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(80, 58);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(54, 13);
            this.label11.TabIndex = 7;
            this.label11.Text = "Yaw Rate";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(80, 16);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(41, 13);
            this.label8.TabIndex = 2;
            this.label8.Text = "YAccel";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(5, 16);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(41, 13);
            this.label7.TabIndex = 1;
            this.label7.Text = "XAccel";
            // 
            // PtcVOutput
            // 
            this.PtcVOutput.Location = new System.Drawing.Point(6, 74);
            this.PtcVOutput.Name = "PtcVOutput";
            this.PtcVOutput.Size = new System.Drawing.Size(74, 20);
            this.PtcVOutput.TabIndex = 9;
            // 
            // XAccOutput
            // 
            this.XAccOutput.Location = new System.Drawing.Point(6, 35);
            this.XAccOutput.Name = "XAccOutput";
            this.XAccOutput.Size = new System.Drawing.Size(71, 20);
            this.XAccOutput.TabIndex = 0;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(6, 58);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(57, 13);
            this.label10.TabIndex = 6;
            this.label10.Text = "Pitch Rate";
            // 
            // testbox2
            // 
            this.testbox2.Location = new System.Drawing.Point(12, 313);
            this.testbox2.Name = "testbox2";
            this.testbox2.Size = new System.Drawing.Size(399, 20);
            this.testbox2.TabIndex = 8;
            this.testbox2.Text = "testing window";
            // 
            // testbox1
            // 
            this.testbox1.Location = new System.Drawing.Point(9, 339);
            this.testbox1.Name = "testbox1";
            this.testbox1.Size = new System.Drawing.Size(399, 20);
            this.testbox1.TabIndex = 4;
            this.testbox1.Text = "testing window";
            // 
            // OutPortFreqNumberBox
            // 
            this.OutPortFreqNumberBox.Location = new System.Drawing.Point(89, 103);
            this.OutPortFreqNumberBox.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.OutPortFreqNumberBox.Name = "OutPortFreqNumberBox";
            this.OutPortFreqNumberBox.Size = new System.Drawing.Size(51, 20);
            this.OutPortFreqNumberBox.TabIndex = 9;
            this.OutPortFreqNumberBox.Value = new decimal(new int[] {
            10,
            0,
            0,
            0});
            // 
            // UDPRoutingGroupBox
            // 
            this.UDPRoutingGroupBox.Controls.Add(this.label4);
            this.UDPRoutingGroupBox.Controls.Add(this.InPortFreqNumberBox);
            this.UDPRoutingGroupBox.Controls.Add(this.InPortNumberBox);
            this.UDPRoutingGroupBox.Controls.Add(this.OutPortNumberBox);
            this.UDPRoutingGroupBox.Controls.Add(this.OutPortFreqNumberBox);
            this.UDPRoutingGroupBox.Controls.Add(this.label22);
            this.UDPRoutingGroupBox.Controls.Add(this.label21);
            this.UDPRoutingGroupBox.Controls.Add(this.label20);
            this.UDPRoutingGroupBox.Controls.Add(this.IPAddressTextBox);
            this.UDPRoutingGroupBox.Controls.Add(this.label19);
            this.UDPRoutingGroupBox.Location = new System.Drawing.Point(268, 76);
            this.UDPRoutingGroupBox.Name = "UDPRoutingGroupBox";
            this.UDPRoutingGroupBox.Size = new System.Drawing.Size(146, 130);
            this.UDPRoutingGroupBox.TabIndex = 2;
            this.UDPRoutingGroupBox.TabStop = false;
            this.UDPRoutingGroupBox.Text = "UDP connection setup";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(11, 60);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(57, 13);
            this.label4.TabIndex = 13;
            this.label4.Text = "Frequency";
            // 
            // InPortFreqNumberBox
            // 
            this.InPortFreqNumberBox.Location = new System.Drawing.Point(89, 56);
            this.InPortFreqNumberBox.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.InPortFreqNumberBox.Name = "InPortFreqNumberBox";
            this.InPortFreqNumberBox.Size = new System.Drawing.Size(51, 20);
            this.InPortFreqNumberBox.TabIndex = 12;
            this.InPortFreqNumberBox.Value = new decimal(new int[] {
            10,
            0,
            0,
            0});
            // 
            // InPortNumberBox
            // 
            this.InPortNumberBox.Location = new System.Drawing.Point(53, 36);
            this.InPortNumberBox.Maximum = new decimal(new int[] {
            2147483647,
            0,
            0,
            0});
            this.InPortNumberBox.Name = "InPortNumberBox";
            this.InPortNumberBox.Size = new System.Drawing.Size(88, 20);
            this.InPortNumberBox.TabIndex = 11;
            this.InPortNumberBox.Value = new decimal(new int[] {
            5010,
            0,
            0,
            0});
            // 
            // OutPortNumberBox
            // 
            this.OutPortNumberBox.Location = new System.Drawing.Point(57, 77);
            this.OutPortNumberBox.Maximum = new decimal(new int[] {
            2147483647,
            0,
            0,
            0});
            this.OutPortNumberBox.Name = "OutPortNumberBox";
            this.OutPortNumberBox.Size = new System.Drawing.Size(88, 20);
            this.OutPortNumberBox.TabIndex = 10;
            this.OutPortNumberBox.Value = new decimal(new int[] {
            5500,
            0,
            0,
            0});
            // 
            // label22
            // 
            this.label22.AutoSize = true;
            this.label22.Location = new System.Drawing.Point(6, 110);
            this.label22.Name = "label22";
            this.label22.Size = new System.Drawing.Size(57, 13);
            this.label22.TabIndex = 6;
            this.label22.Text = "Frequency";
            // 
            // label21
            // 
            this.label21.AutoSize = true;
            this.label21.Location = new System.Drawing.Point(5, 79);
            this.label21.Name = "label21";
            this.label21.Size = new System.Drawing.Size(46, 13);
            this.label21.TabIndex = 5;
            this.label21.Text = "Out Port";
            // 
            // label20
            // 
            this.label20.AutoSize = true;
            this.label20.Location = new System.Drawing.Point(9, 36);
            this.label20.Name = "label20";
            this.label20.Size = new System.Drawing.Size(38, 13);
            this.label20.TabIndex = 4;
            this.label20.Text = "In Port";
            // 
            // IPAddressTextBox
            // 
            this.IPAddressTextBox.Location = new System.Drawing.Point(65, 19);
            this.IPAddressTextBox.MaxLength = 9;
            this.IPAddressTextBox.Name = "IPAddressTextBox";
            this.IPAddressTextBox.Size = new System.Drawing.Size(76, 20);
            this.IPAddressTextBox.TabIndex = 1;
            this.IPAddressTextBox.Text = "127.0.0.1";
            // 
            // label19
            // 
            this.label19.AutoSize = true;
            this.label19.Location = new System.Drawing.Point(10, 19);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(58, 13);
            this.label19.TabIndex = 0;
            this.label19.Text = "IP Address";
            // 
            // FGCStartStopButton
            // 
            this.FGCStartStopButton.Location = new System.Drawing.Point(298, 16);
            this.FGCStartStopButton.Name = "FGCStartStopButton";
            this.FGCStartStopButton.Size = new System.Drawing.Size(69, 54);
            this.FGCStartStopButton.TabIndex = 3;
            this.FGCStartStopButton.Text = "Start";
            this.FGCStartStopButton.UseVisualStyleBackColor = true;
            this.FGCStartStopButton.Click += new System.EventHandler(this.FGCStartStopButton_Click);
            // 
            // HardwareGroupBox
            // 
            this.HardwareGroupBox.Controls.Add(this.label13);
            this.HardwareGroupBox.Controls.Add(this.SerialFreqNumberBox);
            this.HardwareGroupBox.Controls.Add(this.BaudRateNumberBox);
            this.HardwareGroupBox.Controls.Add(this.label6);
            this.HardwareGroupBox.Controls.Add(this.SerialPortSearchButton);
            this.HardwareGroupBox.Controls.Add(this.label5);
            this.HardwareGroupBox.Controls.Add(this.SerialPortListBox);
            this.HardwareGroupBox.Enabled = false;
            this.HardwareGroupBox.Location = new System.Drawing.Point(415, 12);
            this.HardwareGroupBox.Name = "HardwareGroupBox";
            this.HardwareGroupBox.Size = new System.Drawing.Size(137, 182);
            this.HardwareGroupBox.TabIndex = 1;
            this.HardwareGroupBox.TabStop = false;
            this.HardwareGroupBox.Text = "Hardware interface";
            // 
            // HardwareStartStopButton
            // 
            this.HardwareStartStopButton.Enabled = false;
            this.HardwareStartStopButton.Location = new System.Drawing.Point(422, 194);
            this.HardwareStartStopButton.Name = "HardwareStartStopButton";
            this.HardwareStartStopButton.Size = new System.Drawing.Size(117, 20);
            this.HardwareStartStopButton.TabIndex = 0;
            this.HardwareStartStopButton.Text = "Start";
            this.HardwareStartStopButton.UseVisualStyleBackColor = true;
            this.HardwareStartStopButton.Click += new System.EventHandler(this.HardwareStartStopButton_Click);
            // 
            // SerialPortListBox
            // 
            this.SerialPortListBox.FormattingEnabled = true;
            this.SerialPortListBox.Location = new System.Drawing.Point(6, 31);
            this.SerialPortListBox.Name = "SerialPortListBox";
            this.SerialPortListBox.ScrollAlwaysVisible = true;
            this.SerialPortListBox.Size = new System.Drawing.Size(120, 43);
            this.SerialPortListBox.TabIndex = 1;
            this.SerialPortListBox.SelectedIndexChanged += new System.EventHandler(this.SerialPortListBox_SelectedIndexChanged);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(7, 15);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(55, 13);
            this.label5.TabIndex = 2;
            this.label5.Text = "Serial Port";
            // 
            // SerialPortSearchButton
            // 
            this.SerialPortSearchButton.Location = new System.Drawing.Point(7, 81);
            this.SerialPortSearchButton.Name = "SerialPortSearchButton";
            this.SerialPortSearchButton.Size = new System.Drawing.Size(119, 23);
            this.SerialPortSearchButton.TabIndex = 3;
            this.SerialPortSearchButton.Text = "Search Ports";
            this.SerialPortSearchButton.UseVisualStyleBackColor = true;
            this.SerialPortSearchButton.Click += new System.EventHandler(this.SerialPortSearchButton_Click);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(7, 111);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(58, 13);
            this.label6.TabIndex = 4;
            this.label6.Text = "Baud Rate";
            // 
            // BaudRateNumberBox
            // 
            this.BaudRateNumberBox.Location = new System.Drawing.Point(7, 128);
            this.BaudRateNumberBox.Maximum = new decimal(new int[] {
            230400,
            0,
            0,
            0});
            this.BaudRateNumberBox.Minimum = new decimal(new int[] {
            300,
            0,
            0,
            0});
            this.BaudRateNumberBox.Name = "BaudRateNumberBox";
            this.BaudRateNumberBox.Size = new System.Drawing.Size(120, 20);
            this.BaudRateNumberBox.TabIndex = 5;
            this.BaudRateNumberBox.Value = new decimal(new int[] {
            9600,
            0,
            0,
            0});
            // 
            // SerialFreqNumberBox
            // 
            this.SerialFreqNumberBox.Location = new System.Drawing.Point(68, 156);
            this.SerialFreqNumberBox.Name = "SerialFreqNumberBox";
            this.SerialFreqNumberBox.Size = new System.Drawing.Size(63, 20);
            this.SerialFreqNumberBox.TabIndex = 6;
            this.SerialFreqNumberBox.Value = new decimal(new int[] {
            10,
            0,
            0,
            0});
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(5, 163);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(57, 13);
            this.label13.TabIndex = 7;
            this.label13.Text = "Frequency";
            // 
            // FGCForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(562, 363);
            this.Controls.Add(this.UDPRoutingGroupBox);
            this.Controls.Add(this.HardwareGroupBox);
            this.Controls.Add(this.testbox2);
            this.Controls.Add(this.testbox1);
            this.Controls.Add(this.FGCStartStopButton);
            this.Controls.Add(this.HardwareStartStopButton);
            this.Controls.Add(this.SimulatorGroupBox);
            this.Controls.Add(this.ControlGroupBox);
            this.Name = "FGCForm";
            this.Text = "Flight Gear Communicator";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.FGCForm_FormClosed);
            this.ControlGroupBox.ResumeLayout(false);
            this.ControlGroupBox.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.AileronsInput)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.RudderInput)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.ControlUpdate)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.ElevatorInput)).EndInit();
            this.SimulatorGroupBox.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.OutPortFreqNumberBox)).EndInit();
            this.UDPRoutingGroupBox.ResumeLayout(false);
            this.UDPRoutingGroupBox.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.InPortFreqNumberBox)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.InPortNumberBox)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.OutPortNumberBox)).EndInit();
            this.HardwareGroupBox.ResumeLayout(false);
            this.HardwareGroupBox.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.BaudRateNumberBox)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.SerialFreqNumberBox)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox ControlGroupBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.GroupBox SimulatorGroupBox;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox XAccOutput;
        private System.Windows.Forms.TextBox ZAccOutput;
        private System.Windows.Forms.Label Z;
        private System.Windows.Forms.TextBox YAccOutput;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.GroupBox UDPRoutingGroupBox;
        private System.Windows.Forms.TextBox IPAddressTextBox;
        private System.Windows.Forms.Label label19;
        private System.Windows.Forms.Label label21;
        private System.Windows.Forms.Label label20;
        private System.Windows.Forms.Button FGCStartStopButton;
        private System.Windows.Forms.TextBox testbox1;
        private System.Windows.Forms.Label label22;
        private System.Windows.Forms.TextBox testbox2;
        private System.Windows.Forms.TextBox RolVOutput;
        private System.Windows.Forms.TextBox YawVOutput;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.TextBox PtcVOutput;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.NumericUpDown OutPortFreqNumberBox;
        private System.Windows.Forms.NumericUpDown OutPortNumberBox;
        private System.Windows.Forms.NumericUpDown InPortNumberBox;
        private System.Windows.Forms.TrackBar ElevatorInput;
        private System.Windows.Forms.TrackBar RudderInput;
        private System.Windows.Forms.TrackBar ControlUpdate;
        private System.Windows.Forms.TrackBar AileronsInput;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.NumericUpDown InPortFreqNumberBox;
        private System.Windows.Forms.GroupBox HardwareGroupBox;
        private System.Windows.Forms.Button HardwareStartStopButton;
        private System.Windows.Forms.Button SerialPortSearchButton;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.ListBox SerialPortListBox;
        private System.Windows.Forms.NumericUpDown BaudRateNumberBox;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.NumericUpDown SerialFreqNumberBox;
        private System.Windows.Forms.Label label13;

    }
}

