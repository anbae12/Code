
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity clock is
	port(
		  clk : IN STD_LOGIC;
		  clk_1kHz, clk_10Hz, clk_1Hz: OUT STD_LOGIC
		  );
end clock;

architecture Behavioral of clock is

begin


-- This should make the custom clock ---
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

clock_scale_1Hz:
process(clk)
	variable Q: Integer range 0 to 50000000;
begin
	if rising_edge(clk) then
		if Q < 50000000 then
		Q := Q + 1;
		clk_1Hz <= '0';
		else
		Q := 1;
		clk_1Hz <= '1';
		end if;
	end if;
end process;

clock_scale_10Hz:
process(clk)
	variable Q: Integer range 0 to 5000000;
begin
	if rising_edge(clk) then
		if Q < 5000000 then
		Q := Q + 1;
		clk_10Hz <= '0';
		else
		Q := 1;
		clk_10Hz <= '1';
		end if;
	end if;
end process;
end Behavioral;