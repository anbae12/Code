
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity mux is
	port(
	clk: 	IN STD_LOGIC;
	s:		OUT STD_LOGIC_VECTOR (1 downto 0) := "00"
	);
end mux;

architecture Behavioral of mux is
begin

process(clk)
variable sel,temp: STD_LOGIC_VECTOR (1 downto 0):= "00";
begin
	if rising_edge(clk) then
		sel := sel +1;
		case sel is
			when "00" =>
				s <= "01";
			when "01" =>
				s <= "10";			
			when "10" =>
				s <= "11";			
			when "11" =>
				s <= "00";
			when others =>
				null;
		end case;
	end if;
end process;

end Behavioral;

