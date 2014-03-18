
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity stop_watch is
	port(
	clk_1kHz: 	IN STD_LOGIC;
	enable: 		IN STD_LOGIC;
	reset:		IN STD_LOGIC;
	timeout:		OUT STD_LOGIC := '0'
	);
end stop_watch;

architecture Behavioral of stop_watch is
begin

process(clk_1kHz)
variable milli_seconds: Integer range 0 to 60000 := 60000;
begin
	if rising_edge(clk_1kHz) then
	if reset = '1' then
		milli_seconds := 60000;
	end if;
	if enable = '1' then
		if milli_seconds /= 0 then 
			milli_seconds := milli_seconds - 1;
			timeout <= '0';
		elsif milli_seconds = 0 then
			timeout <= '1';
		end if;
	end if;
	end if;
end process;

end Behavioral;

