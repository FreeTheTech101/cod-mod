// PS3 Types for now
enum diffculty_s
{
	bronze = 0,
	silver = 1,
	gold = 2,
	platinum = 3
};

typedef struct achievement_s
{
	char* name;
	char* code;
	char* description;
	diffculty_s difficulty;
}achievement_t;

// Some small struct for displaying onscreen.
typedef struct reward_s
{
	int rewardCode;
	int startTime;
	const char* rewardTitle;
	const char* rewardDescription;
	diffculty_s difficulty;
	bool handledOnce;
}reward_t;


#define ACHIEVEMENT_COUNT 50

achievement_t achievements[ACHIEVEMENT_COUNT + 1];

void buildAchievementList()
{
	if(achievements[0].name)
		return;

	achievements[0].name = "Back in the Saddle";
	achievements[0].code = "BACK_IN_THE_SADDLE";
	achievements[0].description = "Help train the local militia.";
	achievements[0].difficulty = bronze;

	achievements[1].name = "Danger Close";
	achievements[1].code = "DANGER_CLOSE";
	achievements[1].description = "Get hand picked for Shepherd's elite squad.";
	achievements[1].difficulty = bronze;

	achievements[2].name = "Cold Shoulder";
	achievements[2].code = "COLD_SHOULDER";
	achievements[2].description = "Infiltrate the snowy mountain side base.";
	achievements[2].difficulty = bronze;

	achievements[3].name = "Tag 'em and bag 'em";
	achievements[3].code = "TAGEM_AND_BAGEM";
	achievements[3].description = "Find Rojas in the Favelas.";
	achievements[3].difficulty = bronze;

	achievements[4].name = "Royale with Cheese";
	achievements[4].code = "ROYAL_WITH_CHEESE";
	achievements[4].description = "Defend Burger Town.";
	achievements[4].difficulty = bronze;

	achievements[5].name = "Soap on a Rope";
	achievements[5].code = "SOAP_ON_A_ROPE";
	achievements[5].description = "Storm the gulag.";
	achievements[5].difficulty = bronze;

	achievements[6].name = "Desperate Times";
	achievements[6].code = "DESPERATE_TIMES";
	achievements[6].description = "Execute the plan to help the Americans.";
	achievements[6].difficulty = bronze;

	achievements[7].name = "Whiskey Hotel";
	achievements[7].code = "HOUSTON_WE_HAVE_A_PROBLEM";
	achievements[7].description = "Take back Whiskey Hotel.";
	achievements[7].difficulty = bronze;

	achievements[8].name = "The Pawn";
	achievements[8].code = "THE_PAWN";
	achievements[8].description = "Assault Makarov's safe house.";
	achievements[8].difficulty = bronze;

	achievements[9].name = "Out of the Frying Pan...";
	achievements[9].code = "OUT_OF_THE_FRYING_PAN";
	achievements[9].description = "Complete the mission in the airplane graveyard.";
	achievements[9].difficulty = bronze;

	achievements[10].name = "For the Record";
	achievements[10].code = "FOR_THE_RECORD";
	achievements[10].description = "Complete the Single Player campaign on any difficulty.";
	achievements[10].difficulty = silver;

	achievements[11].name = "The Price of War";
	achievements[11].code = "THE_PRICE_OF_WAR";
	achievements[11].description = "Complete the single player campaign on Hardened or Veteran Difficulty.";
	achievements[11].difficulty = gold;

	achievements[12].name = "First Day of School";
	achievements[12].code = "FIRST_DAY_OF_SCHOOL";
	achievements[12].description = "Complete 'S.S.D.D' and 'Team Player' on Veteran Difficulty.";
	achievements[12].difficulty = bronze;

	achievements[13].name = "Black Diamond";
	achievements[13].code = "BLACK_DIAMOND";
	achievements[13].description = "Complete 'Cliffhanger' on Veteran Difficulty.";
	achievements[13].difficulty = bronze;

	achievements[14].name = "Turistas";
	achievements[14].code = "TURISTAS";
	achievements[14].description = "Complete 'Takedown' and 'The Hornet's Nest' on Veteran Difficulty.";
	achievements[14].difficulty = silver;

	achievements[15].name = "Red Dawn";
	achievements[15].code = "RED_DAWN";
	achievements[15].description = "Complete 'Wolverines!' and 'Exodus' on Veteran Difficulty.";
	achievements[15].difficulty = silver;

	achievements[16].name = "Prisoner #627";
	achievements[16].code = "PRISONER_627";
	achievements[16].description = "Complete 'The Only Easy Day... Was Yesterday' and 'The Gulag' on Veteran Difficulty.";
	achievements[16].difficulty = silver;

	achievements[17].name = "Ends Justify the Means";
	achievements[17].code = "ENDS_JUSTIFY_THE_MEANS";
	achievements[17].description = "Complete 'Contingency' on Veteran Difficulty";
	achievements[17].difficulty = bronze;

	achievements[18].name = "Homecoming";
	achievements[18].code = "HOME_COMING";
	achievements[18].description = "Complete 'Of Their Own Accord', 'Second Sun', and 'Whiskey Hotel' on Veteran Difficulty.";
	achievements[18].difficulty = silver;

	achievements[19].name = "Queen takes Rook";
	achievements[19].code = "QUEEN_TAKES_ROOK";
	achievements[19].description = "Complete 'Loose Ends' and 'The Enemy of My Enemy' on Veteran Difficulty.";
	achievements[19].difficulty = silver;

	achievements[20].name = "Off the Grid";
	achievements[20].code = "OFF_THE_GRID";
	achievements[20].description = "Complete 'Just Like Old Times' and 'Endgame' on Veteran Difficulty.";
	achievements[20].difficulty = bronze;

	achievements[21].name = "Pit Boss";
	achievements[21].code = "PIT_BOSS";
	achievements[21].description = "Run The Pit in 'S.S.D.D' and finish with a final time under 30 seconds.";
	achievements[21].difficulty = bronze;

	achievements[22].name = "Ghost";
	achievements[22].code = "GHOST";
	achievements[22].description = "Plant the C4 in 'Cliffhanger' without alerting or injuring anyone in the blizzard.";
	achievements[22].difficulty = bronze;

	achievements[23].name = "Colonel Sanderson";
	achievements[23].code = "COLONEL_SANDERSON";
	achievements[23].description = "Kill 7 chickens in under 10 seconds in 'The Hornet's Nest'.";
	achievements[23].difficulty = bronze;

	achievements[24].name = "Gold Star";
	achievements[24].code = "GOLD_STAR";
	achievements[24].description = "Earn 1 star in Special Ops.";
	achievements[24].difficulty = bronze;

	achievements[25].name = "Hotel Bravo";
	achievements[25].code = "HOTEL_BRAVO";
	achievements[25].description = "Earn 4 stars in Special Ops.";
	achievements[25].difficulty = bronze;

	achievements[26].name = "Charlie On Our Six";
	achievements[26].code = "CHARLIE_ON_OUR_SIX";
	achievements[26].description = "Earn 8 stars in Special Ops.";
	achievements[26].difficulty = bronze;

	achievements[27].name = "It Goes to Eleven";
	achievements[27].code = "IT_GOES_TO_ELEVEN";
	achievements[27].description = "Earn at least 1 star in 11 different Special Op missions.";
	achievements[27].difficulty = bronze;

	achievements[28].name = "Operational Asset";
	achievements[28].code = "OPERATIONAL_ASSET";
	achievements[28].description = "Earn all 3 stars in at least 5 different Special Op missions.";
	achievements[28].difficulty = bronze;

	achievements[29].name = "Blackjack";
	achievements[29].code = "BLACKJACK";
	achievements[29].description = "Earn 21 stars in Special Ops.";
	achievements[29].difficulty = bronze;

	achievements[30].name = "Honor Roll";
	achievements[30].code = "HONOR_ROLL";
	achievements[30].description = "Earn at least 1 star in each Special Op mission.";
	achievements[30].difficulty = silver;

	achievements[31].name = "Operative";
	achievements[31].code = "OPERATIVE";
	achievements[31].description = "Earn all 3 stars in at least 10 different Special Op missions.";
	achievements[31].difficulty = silver;

	achievements[32].name = "Specialist";
	achievements[32].code = "SPECIALIST";
	achievements[32].description = "Earn 30 stars in Special Ops.";
	achievements[32].difficulty = silver;

	achievements[33].name = "Professional";
	achievements[33].code = "PROFESSIONAL";
	achievements[33].description = "Earn all 3 stars in at least 15 different Special Op missions.";
	achievements[33].difficulty = silver;

	achievements[34].name = "Star 69";
	achievements[34].code = "STAR_69";
	achievements[34].description = "Earn 69 stars in Special Ops.";
	achievements[34].difficulty = gold;

	achievements[35].name = "Three-some";
	achievements[35].code = "THREESOME";
	achievements[35].description = "Kill at least 3 enemies with a single shot from a grenade launcher in Single Player or Special Ops.";
	achievements[35].difficulty = bronze;

	achievements[36].name = "Downed but Not Out";
	achievements[36].code = "DOWNED_BUT_NOT_OUT";
	achievements[36].description = "Kill 4 enemies in a row while downed in Special Ops.";
	achievements[36].difficulty = bronze;

	achievements[37].name = "No Rest For the Wary";
	achievements[37].code = "NO_REST_FOR_THE_WARY";
	achievements[37].description = "Knife an enemy without him ever knowing you were there in Single Player or Special Ops.";
	achievements[37].difficulty = bronze;

	achievements[38].name = "I'm the Juggernaut...";
	achievements[38].code = "IM_THE_JUGGERNAUT";
	achievements[38].description = "Kill a Juggernaut in Special Ops.";
	achievements[38].difficulty = bronze;

	achievements[39].name = "Desperado";
	achievements[39].code = "ONE_MAN_ARMY";
	achievements[39].description = "Kill 5 enemies in a row using 5 different weapons or attachments in Single Player or Special Ops.";
	achievements[39].difficulty = bronze;

	achievements[40].name = "Ten plus foot-mobiles";
	achievements[40].code = "TEN_PLUS_FOOT_MOBILES";
	achievements[40].description = "Kill at least 10 enemies with one Predator missile in Single Player or Special Ops.";
	achievements[40].difficulty = bronze;

	achievements[41].name = "Unnecessary Roughness";
	achievements[41].code = "UNNECESSARY_ROUGHNESS";
	achievements[41].description = "Use a riot shield to beat down an enemy in Single Player or Special Ops.";
	achievements[41].difficulty = bronze;

	achievements[42].name = "Knock-knock";
	achievements[42].code = "KNOCK_KNOCK";
	achievements[42].description = "Kill 4 enemies with 4 shots during a slow-mo breach in Single Player or Special Ops.";
	achievements[42].difficulty = bronze;

	achievements[43].name = "Look Ma Two Hands";
	achievements[43].code = "LOOK_MA_TWO_HANDS";
	achievements[43].description = "Kill 10 enemies in a row using akimbo weapons in Single Player or Special Ops.";
	achievements[43].difficulty = bronze;

	achievements[44].name = "Some Like it Hot";
	achievements[44].code = "SOME_LIKE_IT_HOT";
	achievements[44].description = "Kill 6 enemies in a row using a thermal weapon in Single Player or Special Ops.";
	achievements[44].difficulty = bronze;

	achievements[45].name = "Two Birds with One Stone";
	achievements[45].code = "TWO_BIRDS_WITH_ONE_STONE";
	achievements[45].description = "Kill 2 enemies with a single bullet in Single Player or Special Ops.";
	achievements[45].difficulty = bronze;

	achievements[46].name = "The Road Less Traveled";
	achievements[46].code = "THE_ROAD_LESS_TRAVELED";
	achievements[46].description = "Collect 22 enemy intel items.";
	achievements[46].difficulty = bronze;

	achievements[47].name = "Leave No Stone Unturned";
	achievements[47].code = "LEAVE_NO_STONE_UNTURNED";
	achievements[47].description = "Collect 45 enemy intel items.";
	achievements[47].difficulty = bronze;

	achievements[48].name = "Drive By";
	achievements[48].code = "DRIVE_BY";
	achievements[48].description = "Kill 20 enemies in a row while driving a vehicle in Single Player or Special Ops.";
	achievements[48].difficulty = bronze;

	achievements[49].name = "The Harder They Fall";
	achievements[49].code = "THE_HARDER_THEY_FALL";
	achievements[49].description = "Kill 2 rappelling enemies in a row before they land on their feet in Single Player or Special Ops.";
	achievements[49].difficulty = bronze;

	// Extra ;)
	achievements[50].name = "Is That All You Got?";
	achievements[50].code = "PLATINUM";
	achievements[50].description = "Earn all available achievements for Modern Warfare® 2."; // Probably '®' will encounter encoding errors...
	achievements[50].difficulty = platinum;
}