
--------------------------------------------------------
--
--	This component gets input from the hallsensor on the 
-- system. When the system is moving one way it sets a 
-- reset signal high on rising edge on the hallsensor.
-- When the system is moving the other way it sets the
-- reset signal high on falling edge on the hallsensor.
--
--------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use IEEE.NUMERIC_STD.all;

entity decoder_reset is
		port(	
				clk:				IN  STD_LOGIC;
				Hallsensor:		IN  STD_LOGIC;
				direction:		IN  STD_LOGIC;
				reset:			OUT STD_LOGIC
			);
end decoder_reset;

architecture Behavioral of decoder_reset is
signal hallsensor_delayed: 			STD_LOGIC_VECTOR(1 downto 0):= "00";
signal first_reset:						STD_LOGIC:='0';
begin

process(clk)
begin
	if rising_edge(clk) then
		hallsensor_delayed <= hallsensor_delayed(0) & hallsensor;
	end if;
end process;

process(clk)
begin
	if rising_edge(clk) then
		if first_reset = '0' then
			if direction = '1' and hallsensor_delayed = "01" then
				reset <= '1';
				first_reset <= '1';
			elsif direction = '0' and hallsensor_delayed = "10" then
				reset <= '1';
				first_reset <= '1';
			else 
				reset <= '0';
			end if;
		else
			reset <= '0';
		end if;
	end if;
end process;
	
end Behavioral;
