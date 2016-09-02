#include "SDS021.h"

namespace Components
{

	SDS021::SDS021(byte rx_pin, byte tx_pin) : SoftwareSerial_(rx_pin, tx_pin)
	{
		ID_ = 0xFFFF;
		PM2_5_ = 0.0;
		PM10_ = 0.0;
		Mode_ = false;
		State_ = true;
		Interval_ = 0;
		Firmware_ = { 15, 7, 10 };
	}


	SDS021::~SDS021() {}

	void SDS021::Begin()
	{
		SoftwareSerial_.begin(kBaudRate_);
	}

	int SDS021::ID()
	{
		return ID_;
	}

	void SDS021::ID(int id)
	{
		ID_ = id;
	}

	void SDS021::ID(int old_id, int new_id)
	{
		byte* buffer = MakeMessage(EAction::Id, old_id);
		buffer[13] = new_id >> 8;
		buffer[14] = new_id & 0xFF;
		WriteMessage(buffer);
	}

	bool SDS021::PassiveMode()
	{
		return Mode_;
	}

	void SDS021::PassiveMode(bool passive)
	{
		byte* buffer = MakeMessage(EAction::Mode, ID_);
		buffer[4] = passive;
		WriteMessage(buffer);
	}

	bool SDS021::Awake()
	{
		return State_;
	}

	void SDS021::Awake(bool working)
	{
		byte* buffer = MakeMessage(EAction::State, ID_);
		buffer[4] = working;
		WriteMessage(buffer);
	}

	byte SDS021::Interval()
	{
		return Interval_;
	}

	void SDS021::Interval(byte minutes)
	{
		// Clamps minutes between 0-30
		if (minutes > 30)
			minutes = 30;
		byte* buffer = MakeMessage(EAction::Interval, ID_);
		buffer[4] = minutes;
		WriteMessage(buffer);
	}

	float SDS021::PM2_5()
	{
		return PM2_5_;
	}

	float SDS021::PM10()
	{
		return PM10_;
	}

	void SDS021::Query()
	{
		byte* buffer = MakeMessage(EAction::Query, ID_);
		WriteMessage(buffer);
	}

	bool SDS021::Update()
	{
		byte buffer[kInputLength_];

		// Check serial buffer for a message
		if (SoftwareSerial_.available() >= 10)
		{
			// Read and validate the message
			SoftwareSerial_.readBytesUntil((byte)EMessage::Tail, buffer, kInputLength_);

			if (CheckSum(buffer))
			{
				ECommandId command_id = (ECommandId)buffer[1];
				int device_id = CrunchBytes(buffer[6], buffer[7]);

				// Parse data frame
				if (command_id == ECommandId::Data)
				{
					PM2_5_ = (float)CrunchBytes(buffer[3], buffer[2]) / 10;
					PM10_ = (float)CrunchBytes(buffer[5], buffer[4]) / 10;
				}

				// Parse reply frame
				else if (command_id == ECommandId::Reply)
				{
					EAction action = (EAction)buffer[2];
					
					if (action == EAction::Mode)
						Mode_ = buffer[4];
					else if (action == EAction::State)
						State_ = buffer[4];
					else if (action == EAction::Interval)
						Interval_ = buffer[4];
					else if (action == EAction::Version)
						Firmware_ = { buffer[3], buffer[4], buffer[5] };
				}

				return true;
			}
		}

		return false;
	}

	byte* SDS021::MakeMessage(EAction action, int address)
	{
		// Creates a new byte array of zeroes
		byte* buffer = new byte[kOutputLength_] { 0 };

		// Sets common message values
		buffer[0] = (byte)EMessage::Head;
		buffer[1] = (byte)ECommandId::Command;
		buffer[2] = (byte)action;
		buffer[15] = address >> 8;
		buffer[16] = address & 0xFF;
		buffer[kOutputLength_ - 1] = (byte)EMessage::Tail;

		return buffer;
	}

	void SDS021::WriteMessage(byte* buffer)
	{
		// Calculates and stores checksum in output buffer
		buffer[17] = calcCheckSum(buffer, 2, 17);

		SoftwareSerial_.write(buffer, kOutputLength_);

		// Waits for software serial to finish sending message
		delay(16); // (19 bytes * 8 bits * 0.104 ms = 15.808 ms)

		delete[] buffer;
	}

	byte SDS021::calcCheckSum(byte* buffer, int start_idx, int stop_idx)
	{
		int chk = 0;
		for (int i = start_idx; i < stop_idx; i++)
			chk += buffer[i];
		return chk % 256;
	}

	bool SDS021::CheckSum(byte* buffer)
	{
		 return calcCheckSum(buffer, 2, 8) == buffer[8];
	}

	int SDS021::CrunchBytes(byte high_byte, byte low_byte)
	{
		return ((int)high_byte * 0xFF) + (int)low_byte;
	}

}
