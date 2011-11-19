using System;
using System.IO.Ports;
    //needed for serial communication
using System.Threading;
    //needed for multitasking

namespace FlightGear_Communicator
{
    class SerialDataHandler
    {
        public delegate void RecordingDelegate(byte[] Data);
            /*give this class the means to call an outside method with the
              data collected from the serial com port for the purpose of
              performing some operation on the data (such as recording it)*/

        private SerialPort Transceiver;
            //serial port communication capability
        private Thread SpeakThread;
        private Thread ListenThread;
        private ReaderWriterLockSlim SpeakLock =
                new ReaderWriterLockSlim();
        private ReaderWriterLockSlim ListenLock =
                new ReaderWriterLockSlim();
            //threading and synchronization for continuous operations

        private byte[] DataOut;
        private byte[] DataIn;
        private RecordingDelegate Recorder;
        private int OutDelay;
        private int InDelay;
            //storage for cross-thread data transfer

        public SerialDataHandler(string PortName)
        {
            Transceiver = new SerialPort(PortName);
                //the port name tells which register to send the data to,
                //the baud rate is used to sync the data and ensure
                //correct decoding
            Transceiver.BaudRate = 9600;
            Transceiver.Open();
        }
            //constructor sets com parameters and opens port

        ~SerialDataHandler()
        {
            StopSpeaking();
            StopListening();
            Transceiver.Close();
        }
            //destructor closes port and releases resources

        public void Format(int Baud)
        {
            Transceiver.BaudRate = Baud;
            Transceiver.DataBits = 8;
            Transceiver.Parity = Parity.None;
            Transceiver.StopBits = StopBits.One;
        }

        public bool IsConnected
        {
            get
            {
                return Transceiver.IsOpen;
            }
            set
            {
                switch (value)
                {
                    case true:
                        Transceiver.Open();
                        break;
                    case false:
                        Transceiver.Close();
                        break;
                }
                    //toggle connection to reflect user input
            }
        }
            //property gets or sets the connection status

        public bool IsSpeaking
        {
            get
            {
                if (SpeakThread == null)
                {
                    return false;
                }
                else
                {
                    return SpeakThread.IsAlive;
                }
            }
        }
            //property gets the status of continuous mode data sending

        public bool IsListening
        {
            get
            {
                if (ListenThread == null)
                {
                    return false;
                }
                else
                {
                    return ListenThread.IsAlive;
                }
            }
        }
            //property gets the status of continuous mode data retrieval

        public static string[] ScanPorts()
        {
            return SerialPort.GetPortNames();
        }
            /*method returns an array of strings for the names of the
              available com ports without constructing the class object*/

        public bool Send(byte[] Data)
        {
            if (Data.Length > Transceiver.WriteBufferSize)
            {
                Transceiver.WriteBufferSize = Data.Length;
            }
                //make sure the buffer is big enough
            if ((Transceiver.WriteBufferSize - Data.Length)
                    >= Transceiver.BytesToWrite)
            {
                Transceiver.Write(Data, 0, Data.Length);
                return true;
            }
            else
            {
                return false;
            }
                //if there is enough room in the buffer write the data and
                //report whether write was successful
        }
            /*basic output method for one packet with checks for buffer
              size, returns true if send was successful*/

        public void Speak(byte[] Data)
        {
            if (Data.Length > Transceiver.WriteBufferSize)
            {
                Transceiver.WriteBufferSize = Data.Length;
            }
                //make sure the buffer is big enough
            SpeakLock.EnterWriteLock();
                DataOut = Data;
                OutDelay = 0;
            SpeakLock.ExitWriteLock();
                //hang until access to the variables "DataOut" and
                //"OutDelay" are secured for writing for this thread, then
                //restore general access
            if (SpeakThread == null)
            {
                SpeakThread = new Thread(SpeakLoop);
                SpeakThread.Start();
            }
                //keep the thread running unless asked to stop
        }
        public void Speak(byte[] Data, int Frequency)
        {
            if (Data.Length > Transceiver.WriteBufferSize)
            {
                Transceiver.WriteBufferSize = Data.Length;
            }
                //make sure the buffer is big enough
            SpeakLock.EnterWriteLock();
                DataOut = Data;
                OutDelay = 1000 / Frequency;
            SpeakLock.ExitWriteLock();
                //hang until access to the variables "DataOut" and
                //"OutDelay" are secured for writing for this thread, then
                //restore general access
            if (SpeakThread == null)
            {
                SpeakThread = new Thread(SpeakLoop);
                SpeakThread.Start();
            }
                //keep the thread running unless asked to stop
        }
            /*method sends data continuously with overloads to send as
              fast as possible or at the specified frequency, repeat calls
              only upate data and frequency;*/

        private void SpeakLoop()
        {
            int Pause;
                //sleep outside the lock to prevent threadlock
            while (true)
            {
                SpeakLock.EnterReadLock();
                    Pause = OutDelay;
                    Transceiver.Write(DataOut, 0, DataOut.Length);
                SpeakLock.ExitReadLock();
                    //hang until access to the variable "OutDelay" and the
                    //object "Transceiver" are secured for reading for
                    //this thread, then restore general access
                Thread.Sleep(Pause);
                    //hang the execution of the loop intermittently to
                    //create the desired data-sending frequency
            }
        }
            //continuous data sending subroutine

        public void StopSpeaking()
        {
            if (SpeakThread != null)
            {
                SpeakThread.Abort();
                    //use the Abort method to close the "SpeakThread"
                if (SpeakLock.IsReadLockHeld)
                {
                    SpeakLock.ExitReadLock();
                }
                    //release readerlock if it remains after the abort
                SpeakThread = null;
                    //return "SpeakThread" to null
            }
                //check if "SpeakThread" has been constructed
        }
            //terminate the "SpeakThread"

        public byte[] Receive()
        {
            int BufferSize = Transceiver.BytesToRead;
                //the size could change between clock cycles
            byte[] Data = new byte[BufferSize];
            Transceiver.Read(Data, 0, BufferSize);
            return Data;
        }
        public void Receive(RecordingDelegate Delegate)
        {
            int BufferSize = Transceiver.BytesToRead;
                //the amount could change between clock cycles
            byte[] Data = new byte[BufferSize];
            Transceiver.Read(Data, 0, BufferSize);
            Delegate(Data);
        }
        public byte[] Receive(int PacketSize)
        {
            if (PacketSize > Transceiver.ReadBufferSize)
            {
                Transceiver.ReadBufferSize = PacketSize;
            }
                //make sure the buffer is big enough
            if (PacketSize <= Transceiver.BytesToRead)
            {
                byte[] Data = new byte[PacketSize];
                Transceiver.Read(Data, 0, PacketSize);
                return Data;
            }
                //read a packet if there's a whole packet available
            else
            {
                byte[] Data = new byte[0];
                return Data;
            }
                //otherwise return an empty array
        }
        public void Receive(RecordingDelegate Delegate, int PacketSize)
        {
            if (PacketSize > Transceiver.ReadBufferSize)
            {
                Transceiver.ReadBufferSize = PacketSize;
            }
                //make sure the buffer is big enough
            if (PacketSize <= Transceiver.BytesToRead)
            {
                byte[] Data = new byte[PacketSize];
                Transceiver.Read(Data, 0, PacketSize);
                Delegate(Data);
            }
                //read a packet if there's a whole packet available
            else
            {
                byte[] Data = new byte[0];
                Delegate(Data);
            }
            //otherwise call record with an empty array
        }
            /*basic input method with overload, return entire buffer or a
              specified packet of queued bytes, or call an outside
              recorder method with the entire buffer or with a specified
              packet; if the desired packet is too big the method sends an
              empty array*/

        public void Listen(RecordingDelegate Delegate)
        {
            ListenLock.EnterWriteLock();
                DataIn = new byte[1];
                Recorder = Delegate;
                InDelay = 0;
            ListenLock.ExitWriteLock();
                //hang until access to the variable "InDelay" and the
                //delegate "Recorder" are secured for writing for this
                //thread, then restore general access
            if (ListenThread == null)
            {
                ListenThread = new Thread(ListenLoop);
                ListenThread.Start();
            }
                //keep the thread running unless asked to stop
        }
        public void Listen(RecordingDelegate Delegate, int PacketSize)
        {
            if (PacketSize > Transceiver.ReadBufferSize)
            {
                Transceiver.ReadBufferSize = PacketSize;
            }
                //make sure the buffer is big enough
            ListenLock.EnterWriteLock();
                DataIn = new byte[PacketSize];
                Recorder = Delegate;
                InDelay = 0;
            ListenLock.ExitWriteLock();
                //hang until access to the variables "InDelay" and
                //"DataIn" and the delegate "Recorder" are secured for
                //writing for this thread, then restore general access
            if (ListenThread == null)
            {
                ListenThread = new Thread(ListenLoop);
                ListenThread.Start();
            }
                //keep the thread running unless asked to stop
        }
        public void Listen(
                RecordingDelegate Delegate, int PacketSize, int Frequency)
        {
            if (PacketSize > Transceiver.ReadBufferSize)
            {
                Transceiver.ReadBufferSize = PacketSize;
            }
                //make sure the buffer is big enough
            ListenLock.EnterWriteLock();
                DataIn = new byte[PacketSize];
                Recorder = Delegate;
                InDelay = 1000 / Frequency;
            ListenLock.ExitWriteLock();
                //hang until access to the variables "InDelay" and
                //"DataIn" and the delegate "Recorder" are secured for
                //writing for this thread, then restore general access
            if (ListenThread == null)
            {
                ListenThread = new Thread(ListenLoop);
                ListenThread.Start();
            }
                //keep the thread running unless asked to stop
        }
            /*method records data continuously with the specified delegate
              with overloads to read single bytes as fast as possible and
              specified packets as fast as possible or at a specified
              frequency, repeat calls only upate recording method, packet
              size, and frequency*/

        private void ListenLoop()
        {
            int Pause;
                //sleep outside the lock to prevent threadlock
            while (true)
            {
                ListenLock.EnterReadLock();
                    Pause = InDelay;
                    if (Transceiver.BytesToRead >= DataIn.Length)
                    {
                        Transceiver.Read(DataIn, 0, DataIn.Length);
                    }
                        //read a packet if there is a whole one available
                    Recorder(DataIn);
                ListenLock.ExitReadLock();
                    //hang until access to the variable "InDelay", the
                    //object "Transceiver", and the delegate "Recorder"
                    //are secured for reading for this thread, then
                    //restore general access
                Thread.Sleep(Pause);
                    //hang the execution of the loop intermittently to
                    //create the desired data-receiving frequency
            }
        }
            /*continuous data receiving subroutine, only read if there is
              an entire packet in buffer; if not reading, send old data*/

        public void StopListening()
        {
            if (ListenThread != null)
            {
                ListenThread.Abort();
                    //use the Abort method to close the "ListenThread"
                if (ListenLock.IsReadLockHeld)
                {
                    ListenLock.ExitReadLock();
                }
                    //release readerlock if it remains after the abort
                ListenThread = null;
            }
                //check if "ListenThread" has been constructed
        }
            //terminate the "ListenThread"

        public void Close()
        {
            StopSpeaking();
            StopListening();
            Transceiver.Close();
        }
            //function to end communication
    }
}