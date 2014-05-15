
--------------------------------------------------------
--
--	This component is in charge of putting the two 
-- motorcounts in the DataOut alternately.
--
--------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity output_decoder is
		port(									
			  SS: 					IN  STD_LOGIC;									
			  count_m1:				IN  STD_LOGIC_VECTOR(10 downto 0);
			  count_m2: 			IN  STD_LOGIC_VECTOR(10 downto 0);
			  DataOut: 				OUT STD_LOGIC_VECTOR (15 downto 0)
			  );
end output_decoder;

architecture Behavioral of output_decoder is
signal motor:		STD_LOGIC:= '0';
begin

process(SS)
begin



	if falling_edge(SS) then
		
		if motor = '0' then
			motor <= '1';
		else 
			motor <= '0';
		end if;
		
		if motor = '0' then
			Dataout <= "0" & "0000" & count_m1;
		else
			Dataout <= "1" & "0000" & count_m2;
		end if;
	end if;
end process;



end Behavioral;

