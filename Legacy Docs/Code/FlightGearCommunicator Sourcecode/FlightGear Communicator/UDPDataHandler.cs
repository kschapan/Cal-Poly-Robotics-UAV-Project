using System;
using System.Net;
    //needed for network addressing
using System.Net.Sockets;
    //needed for UDP communication
using System.Threading;
    //needed for multitasking

namespace FlightGear_Communicator
{
    class UDPDataHandler
    {
        public delegate void RecordingDelegate(byte[] Data);
            /*give this class the means to call an outside method with the
              data collected from the serial com port for the purpose of
              performing some operation on the data (such as recording it)*/

        private UdpClient Transmitter;
        private UdpClient Receiver;
            //UDP communication capability
        private IPEndPoint OutPath;
        private IPEndPoint InPath;
            //UDP addressing
        private Thread SpeakThread;
        private Thread ListenThread;
        private ReaderWriterLockSlim SpeakLock =
                new ReaderWriterLockSlim();
        private ReaderWriterLockSlim ListenLock =
                new ReaderWriterLockSlim();
            //threading and synchronization for continuous operations

        private byte[] DataOut;
        private RecordingDelegate Recorder;
        private int OutDelay;
        private int InDelay;
            //storage for cross-thread data transfer

        public UDPDataHandler(IPAddress DestIPA, int InPort, int OutPort)
        {
            Transmitter = new UdpClient();
                //construct the "Transmitter" object
            Receiver = new UdpClient(InPort);
                //construct the "Receiver" object and bind it to the port
                //that it will recieve data from
            OutPath = new IPEndPoint(DestIPA, OutPort);
                //construct the "OutPath" object from the desired address
                //and port
            InPath = new IPEndPoint(DestIPA, InPort);
                //construct the "InPath" object from the desired address
                //and port
        }
            //constructor sets data paths

        ~UDPDataHandler()
        {
            StopSpeaking();
            StopListening();
            Transmitter.Close();
            Receiver.Close();
        }
            //destructor closes connection and releases resources

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

        public void Send(byte[] Data)
        {
            Transmitter.Send(Data, Data.Length, OutPath);
        }
            //basic output method for one packet

        public void Speak(byte[] Data)
        {
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
                    Transmitter.Send(DataOut, DataOut.Length, OutPath);
                SpeakLock.ExitReadLock();
                    //hang until access to the variable "OutDelay" and the
                    //object "Transmitter" are secured for reading for
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
            return Receiver.Receive(ref InPath);
        }
        public void Receive(RecordingDelegate Delegate)
        {
            Delegate(Receiver.Receive(ref InPath));
        }
            /*basic input method with overload, return a packet, or call
              an outside recorder method with the packet*/

        public void Listen(RecordingDelegate Delegate)
        {
            ListenLock.EnterWriteLock();
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
        public void Listen(RecordingDelegate Delegate, int Frequency)
        {
            ListenLock.EnterWriteLock();
                Recorder = Delegate;
                InDelay = 1000 / Frequency;
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
            /*method records data continuously with the specified delegate
              with overloads to read single bytes as fast as possible or
              at a specified frequency, repeat calls only upate recording
              method, packet size, and frequency*/

        private void ListenLoop()
        {
            int Pause;
                //sleep outside the lock to prevent threadlock
            while (true)
            {
                ListenLock.EnterReadLock();
                    Pause = InDelay;
                    if (Receiver.Available > 0)
                    {
                        Recorder(Receiver.Receive(ref InPath));
                    }
                ListenLock.ExitReadLock();
                    //hang until access to the variable "InDelay", the
                    //object "Receiver", and the delegate "Recorder" are
                    //secured for reading for this thread, then restore
                    //general access
                Thread.Sleep(Pause);
                    //hang the execution of the loop intermittently to
                    //create the desired data-receiving frequency
            }
        }
            //continuous data receiving subroutine

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
            Transmitter.Close();
            Receiver.Close();
        }
            //function to end communication
    }
}