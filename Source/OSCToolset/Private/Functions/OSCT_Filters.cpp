// Fill out your copyright notice in the Description page of Project Settings.

#include "Functions/OSCT_Filters.h"
#include "OSCT_Modules.h"

#include "OSCToolsetLog.h"

void UOSCT_Filters::FilterByAddress(
	const FOSCT_Receiver& InReceiver, 
	const FString& MatchAddress, 
	FOSCT_Receiver& OutReceiver,
	EOSCT_FilterResult& Branches)
{
	OutReceiver = InReceiver;
	if (InReceiver.FormattedAddress.IsEmpty())
	{
		Branches = EOSCT_FilterResult::NoMatch;
		return;
	}
	//Exact match for either the full formatted address or the address of the module.
	if (InReceiver.FormattedAddress == MatchAddress || InReceiver.Address == MatchAddress)
	{
		Branches = EOSCT_FilterResult::Matches;
		return;
	}
	Branches = EOSCT_FilterResult::NoMatch;
}

void UOSCT_Filters::FilterByReceiver(
		const FOSCT_Receiver& InReceiver, 
		const FOSCT_Receiver& MatchReceiver, 
		FOSCT_Receiver& OutReceiver, 
		EOSCT_FilterResult& Branches)
{
	OutReceiver = InReceiver;
	// We check Address, Role, and ModuleType individually, which is inherent to FormattedAddress.
	const bool bMatches = InReceiver.Address.Equals(MatchReceiver.Address, ESearchCase::IgnoreCase) && 
		InReceiver.Role == MatchReceiver.Role &&
		InReceiver.ModuleType == MatchReceiver.ModuleType;
	if (bMatches)
	{
		Branches = EOSCT_FilterResult::Matches;
	}
	else
	{
		Branches = EOSCT_FilterResult::NoMatch;
	}
}