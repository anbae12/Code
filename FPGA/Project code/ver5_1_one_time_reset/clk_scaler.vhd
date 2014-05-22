
--------------------------------------------------------
--
--	This component gets scales the internal 50 MHz clock
-- to a 1kHz clock.
--
--------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;


entity clk_scaler is
	port(
			Clk:				IN  STD_LOGIC;
			Clk_1kHz:		OUT STD_LOGIC
			);
end clk_scaler;

architecture Behavioral of clk_scaler is

begin

clock_scale_1kHz:
process(clk)
	variable Q: Integer range 0 to 50000;
begin
	if rising_edge(clk) then
		if Q < 50000 then
		Q := Q + 1;
		clk_1kHz <= '0';
		else
		Q := 1;
		clk_1kHz <= '1';
		end if;
	end if;
end process;

end Behavioral;

