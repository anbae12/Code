--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   11:24:08 04/04/2014
-- Design Name:   
-- Module Name:   C:/Users/Mikkel Jaedicke/Dropbox/Semesterprojekt F14/FPGA/ver1.2/TB_decoder_reset.vhd
-- Project Name:  project_ver1.1
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: decoder_reset
-- 
-- Dependencies:
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
--
-- Notes: 
-- This testbench has been automatically generated using types std_logic and
-- std_logic_vector for the ports of the unit under test.  Xilinx recommends
-- that these types always be used for the top-level I/O of a design in order
-- to guarantee that the testbench will bind correctly to the post-implementation 
-- simulation model.
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
 
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--USE ieee.numeric_std.ALL;
 
ENTITY TB_decoder_reset IS
END TB_decoder_reset;
 
ARCHITECTURE behavior OF TB_decoder_reset IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT decoder_reset
    PORT(
         clk : IN  std_logic;
         Hallsensor : IN  std_logic;
         direction : IN  std_logic;
         reset : OUT  std_logic
        );
    END COMPONENT;
    

   --Inputs
   signal clk : std_logic := '0';
   signal Hallsensor : std_logic := '0';
   signal direction : std_logic := '0';

 	--Outputs
   signal reset : std_logic;

   -- Clock period definitions
   constant clk_period : time := 10 ns;
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: decoder_reset PORT MAP (
          clk => clk,
          Hallsensor => Hallsensor,
          direction => direction,
          reset => reset
        );

   -- Clock process definitions
   clk_process :process
   begin
		clk <= '0';
		wait for clk_period/2;
		clk <= '1';
		wait for clk_period/2;
   end process;
 

   -- Stimulus process
   stim_proc: process
   begin		
      -- hold reset state for 100 ns.
      wait for 100 ns;	

      wait for clk_period*10;

      -- insert stimulus here 

		wait for 100 ns;
		
		Hallsensor <= '1';
		
		wait for 100 ns;
		
		Hallsensor <= '0';
		
		wait for 100 ns;
		
		Direction <= '1';
		
		wait for 100 ns;
	
		hallsensor <= '1';
		
		wait for 100 ns;
		
		hallsensor <= '0';
		
		wait for 100 ns;
      wait;
   end process;

END;
