# Welcome!
This folder contains all of the work assocated with the Fantastical Mundane crafting system spreadsheet! This project was one of two in the course described below and focused on the hard numbers behind crafting systems and the delicate relationship between them.

## Course Details
**Name:** DES212: System Design Methods
**Instructor:** Benjamin Ellinger
**Semester:** Spring 2022
**Description:** This course is an introduction to game design theory and
the process of designing games. Topics may include design
principles, writing rules, playtesting, game state, randomness,
hidden information, and game balance.

# Downloading the Release Build
Given that this project was contained completely in a spreadsheet, all you have to do to view it is download the Excel file (.xlsx) in the repository!

# Look Into the Spreadsheet
This workbook has many sheets that perform different jobs. I've outlined what each one does below.

## Progression
This sheet takes a look at the different 'pools' of inherit resources the player has (e.g., time, crafting prowess (levels), etc.). There is an extensive explanation cell going over what each relationship entails.

## Recipes
Using the crafting system (outlined in the later sheets) this sheet composes mock acheivements/quests that the player can experiences alongside their rewards (which are recipes in the crafting system). Each quest has a title, flavor text, difficulty level, narrative explanation, and reward table with probabilities.

## Raw
The most basic materials of the crafting system. This sheet outlines each of the base materials alongside their base cost, recipe cast, and cost per unit (1, 10, 100). 

## Refined
These materials are refinements of the raw materials. This sheet outlines the same information as the previous except the base cost is now being calculated from the Cost spreadsheet (discussed later)

## Parts
The first level of crafting, these materials are combinations of the previous two categories. Similar to the Refined, this sheet has the same base information with the base cost being derived from the Cost sheet.

## Product
The final tier of the crafting system. This sheet goes over the final products that the crafting system can yield alongside their base information.

## Cost
Using the items listed above, this sheet determined how much the upper tiers of the crafting system cost. Using a 2D array structure, this sheet let's you easily adjust how many materials go into a product and calculates out how 'difficult' it is to create that product.

## Analysis
This sheet takes a look at the crafting system and performs some high-level analytics on it with visual representations. Each chart has an explanation for the data presented.

## Explanation
This sheet explains the thought process behind the crafting system as a whole. Here you can find explanations to how the character will find these materials, discover the recipes, and relevant details about the world.