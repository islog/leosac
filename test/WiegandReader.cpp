#include <tools/runtimeoptions.hpp>
#include <modules/wiegand/wiegand.hpp>
#include "helper/TestHelper.hpp"

class WiegandReaderTest : public TestHelper
{
private:
  virtual bool run_module(zmqpp::socket *pipe) override
  {
    boost::property_tree::ptree cfg, module_cfg, readers_cfg, reader1_cfg;

    reader1_cfg.add("name", "WIEGAND_1");
    reader1_cfg.add("high", "GPIO_HIGH");
    reader1_cfg.add("low", "GPIO_LOW");

    readers_cfg.add_child("reader", reader1_cfg);
    module_cfg.add_child("readers", readers_cfg);
    cfg.add_child("module_config", module_cfg);

    return test_run_module<WiegandReaderModule>(&ctx_, pipe, cfg);
  }

public:

  WiegandReaderTest() :
    TestHelper(),
    high_(ctx_, "GPIO_HIGH"),
    low_(ctx_, "GPIO_LOW")
  {
    bus_sub_.subscribe("S_WIEGAND_1");
  }

  ~WiegandReaderTest()
  {
  }

  FakeGPIO high_;
  FakeGPIO low_;

    //FakeGPIO greenled_gpio_;
  //  FakeLed greenled_;
};

TEST_F(WiegandReaderTest, readCard)
{
  for (int i = 0 ; i < 32; i++)
    {
      high_.interrupt(); // building card id ff:ff:ff:ff
    }

  ASSERT_TRUE(bus_read(bus_sub_, "S_WIEGAND_1", "ff:ff:ff:ff"));

  for (int i = 0 ; i < 32; i++)
    {
      if (i >= 24)
	high_.interrupt();
      else
	low_.interrupt();

      //required because zmq sockets do fair-queuing.
      // its not a problem either, because the hardware will pause too.
      std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }

  ASSERT_TRUE(bus_read(bus_sub_, "S_WIEGAND_1", "ff:00:00:00"));
}
/*

TEST_F(WiegandReaderTest, controlGreenLed)
{

}*/
