/*
 * Author: doe300
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include <iostream>
#include <iomanip>

#include "src/Mailbox.h"
#include "src/V3D.h"

using namespace vc4cl;

static const size_t NAME_LENGTH = 24;
static const size_t VAL_LENGTH = 16;

static void checkResult(bool result)
{
	if(!result)
		throw std::runtime_error("Error in mailbox-call!");
}

static void printMailboxInfo()
{
	std::vector<unsigned> response;
	std::vector<unsigned> response1;
	std::vector<unsigned> response2;
	Mailbox& mb = mailbox();
	std::cout << "Mailbox Info:" << std::endl;
	checkResult(mb.readMailbox(MailboxTag::FIRMWARE_REVISION, 1, {}, response));
	std::cout << std::setw(NAME_LENGTH) << "Firmware Revision:" << std::setw(VAL_LENGTH) << std::hex << response.at(0) << std::dec << std::endl;
	checkResult(mb.readMailbox(MailboxTag::BOARD_MODEL, 1, {}, response));
	std::cout << std::setw(NAME_LENGTH) << "Board Model:" << std::setw(VAL_LENGTH) << response.at(0) << std::endl;
	checkResult(mb.readMailbox(MailboxTag::BOARD_REVISION, 1, {}, response));
	std::cout << std::setw(NAME_LENGTH) << "Board Revision:" << std::setw(VAL_LENGTH) << std::hex << response.at(0) << std::dec << std::endl;
	checkResult(mb.readMailbox(MailboxTag::MAC_ADDRESS, 2, {}, response));
	//XXX 6 byte MAC
	std::cout << std::setw(NAME_LENGTH) << "MAC Address:" << std::setw(VAL_LENGTH) << std::hex << response.at(0) << std::dec << std::endl;
	checkResult(mb.readMailbox(MailboxTag::BOARD_SERIAL, 2, {}, response));
	std::cout << std::setw(NAME_LENGTH) << "Board Serial:" << std::setw(VAL_LENGTH) << std::hex << response.at(0) << " " << response.at(1) << std::dec << std::endl;

	checkResult(mb.readMailbox(MailboxTag::ARM_MEMORY, 2, {}, response));
	std::cout << std::setw(NAME_LENGTH) << "ARM Memory:" << std::setw(VAL_LENGTH) << response.at(1) << " Bytes (" << (response.at(1) / 1024 / 1024) << " MB)" << std::endl;
	checkResult(mb.readMailbox(MailboxTag::VC_MEMORY, 2, {}, response));
	std::cout << std::setw(NAME_LENGTH) << "VideoCore IV Memory:" << std::setw(VAL_LENGTH) << response.at(1) << " Bytes (" << (response.at(1) / 1024 / 1024) << " MB)" << std::endl;

	checkResult(mb.readMailbox(MailboxTag::GET_CLOCK_RATE, 2, std::vector<unsigned>{static_cast<unsigned>(VC4Clock::ARM)}, response));
	checkResult(mb.readMailbox(MailboxTag::GET_MAX_CLOCK_RATE, 2, std::vector<unsigned>{static_cast<unsigned>(VC4Clock::ARM)}, response1));
	checkResult(mb.readMailbox(MailboxTag::GET_MIN_CLOCK_RATE, 2, std::vector<unsigned>{static_cast<unsigned>(VC4Clock::ARM)}, response2));
	std::cout << std::setw(NAME_LENGTH) << "Clock Rate (ARM):" << std::setw(VAL_LENGTH) << response.at(1) / 1000000 << " MHz (" << response2.at(1) / 1000000 << " to " << response1.at(1) / 1000000 << " MHz)" << std::endl;
	checkResult(mb.readMailbox(MailboxTag::GET_CLOCK_RATE, 2, std::vector<unsigned>{static_cast<unsigned>(VC4Clock::CORE)}, response));
	checkResult(mb.readMailbox(MailboxTag::GET_MAX_CLOCK_RATE, 2, std::vector<unsigned>{static_cast<unsigned>(VC4Clock::CORE)}, response1));
	checkResult(mb.readMailbox(MailboxTag::GET_MIN_CLOCK_RATE, 2, std::vector<unsigned>{static_cast<unsigned>(VC4Clock::CORE)}, response2));
	std::cout << std::setw(NAME_LENGTH) << "Clock Rate (Core):" << std::setw(VAL_LENGTH) << response.at(1) / 1000000 << " MHz (" << response2.at(1) / 1000000 << " to " << response1.at(1) / 1000000 << " MHz)"  << std::endl;
	checkResult(mb.readMailbox(MailboxTag::GET_CLOCK_RATE, 2, std::vector<unsigned>{static_cast<unsigned>(VC4Clock::V3D)}, response));
	checkResult(mb.readMailbox(MailboxTag::GET_MAX_CLOCK_RATE, 2, std::vector<unsigned>{static_cast<unsigned>(VC4Clock::V3D)}, response1));
	checkResult(mb.readMailbox(MailboxTag::GET_MIN_CLOCK_RATE, 2, std::vector<unsigned>{static_cast<unsigned>(VC4Clock::V3D)}, response2));
	std::cout << std::setw(NAME_LENGTH) << "Clock Rate (V3D):" << std::setw(VAL_LENGTH) << response.at(1) / 1000000 << " MHz (" << response2.at(1) / 1000000 << " to " << response1.at(1) / 1000000 << " MHz)"  << std::endl;
	checkResult(mb.readMailbox(MailboxTag::GET_CLOCK_RATE, 2, std::vector<unsigned>{static_cast<unsigned>(VC4Clock::PWM)}, response));
	checkResult(mb.readMailbox(MailboxTag::GET_MAX_CLOCK_RATE, 2, std::vector<unsigned>{static_cast<unsigned>(VC4Clock::PWM)}, response1));
	checkResult(mb.readMailbox(MailboxTag::GET_MIN_CLOCK_RATE, 2, std::vector<unsigned>{static_cast<unsigned>(VC4Clock::PWM)}, response2));
	std::cout << std::setw(NAME_LENGTH) << "Clock Rate (PWM):" << std::setw(VAL_LENGTH) << response.at(1) / 1000000 << " MHz (" << response2.at(1) / 1000000 << " to " << response1.at(1) / 1000000 << " MHz)"  << std::endl;

	checkResult(mb.readMailbox(MailboxTag::GET_TEMPERATURE, 2, std::vector<unsigned>{0}, response));
	checkResult(mb.readMailbox(MailboxTag::GET_MAX_TEMPERATURE, 2, std::vector<unsigned>{0}, response1));
	std::cout << std::setw(NAME_LENGTH) << "SoC Temperature:" << std::setw(VAL_LENGTH) << response.at(1) / 1000 << " C (max " << response1.at(1) / 1000 << " C)"  << std::endl;
}

static void printV3DInfo()
{
	std::cout << "V3D Status Register Info:" << std::endl;
	std::cout << std::setw(NAME_LENGTH) << "VPM Memory size:" << std::setw(VAL_LENGTH) << V3D::instance().getSystemInfo(SystemInfo::VPM_MEMORY_SIZE) / 1024 << " KB" << std::endl;
	std::cout << std::setw(NAME_LENGTH) << "Semaphores:" << std::setw(VAL_LENGTH) << V3D::instance().getSystemInfo(SystemInfo::SEMAPHORES_COUNT) << std::endl;
	std::cout << std::setw(NAME_LENGTH) << "QPUs:" << std::setw(VAL_LENGTH) << V3D::instance().getSystemInfo(SystemInfo::QPU_COUNT) << std::endl;
	std::cout << std::setw(NAME_LENGTH) << "Slices:" << std::setw(VAL_LENGTH) << V3D::instance().getSystemInfo(SystemInfo::SLICES_COUNT) << std::endl;
	std::cout << std::setw(NAME_LENGTH) << "VPM User size:" << std::setw(VAL_LENGTH) << V3D::instance().getSystemInfo(SystemInfo::VPM_USER_MEMORY_SIZE) / 1024 << " KB" << std::endl;

	//TODO queue info (V3D_SRQCS), cache info(V3D_L2CACTL), HDR support(V3D_IDENT1), performance counters, errors
}

int main(int argc, char** argv)
{
	std::cout << "V3D Info:" << std::endl;
	std::cout << std::endl;


	printMailboxInfo();
	printV3DInfo();
}
