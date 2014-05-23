
--------------------------------------------------------
--
--	This component gets 16 bit data from the SPI 
-- component and translates it according to the protocol
-- described in the project.
--
--------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity input_decoder is
	port(
			clk:				IN  STD_LOGIC;
			SPI_data:		IN  STD_LOGIC_VECTOR(15 downto 0);
			PWM_channel:	OUT STD_LOGIC_VECTOR(1 downto 0);
			PWM_threshold:	OUT STD_LOGIC_VECTOR(10 downto 0)	
			);
end input_decoder;

architecture Behavioral of input_decoder is
begin

process(clk)
begin
	if rising_edge(clk) then
		if SPI_data(13) = '1' then		-- THE PWM-SET BIT IS NOT SET
			PWM_channel 	<= SPI_data(15 downto 14);
			PWM_threshold 	<= SPI_data(10 downto 0);
		end if;
	end if;
end process;
end Behavioral;

