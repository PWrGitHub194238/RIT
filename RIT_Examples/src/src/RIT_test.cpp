#include <gtest/gtest.h>
#include <gtest/internal/gtest-port.h>
#include <RIT/log/bundle/Bundle.hpp>
#include <RIT/log/utils/LocaleEnum.hpp>
#include <RIT/log/utils/LogUtils.hpp>
#include <log4cxx/helpers/messagebuffer.h>
#include <log4cxx/logger.h>
#include <string>


int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);

	log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("log"));

	LogUtils::configureLog("Log4cxxConfig.xml");

	LogUtils::setLocale(LocaleEnum::EN_US_UTF8);


	//return 0;
	::testing::GTEST_FLAG(filter) = "USE_CASE.AIMST_ST_EXAMPLE";
	return RUN_ALL_TESTS();
}

