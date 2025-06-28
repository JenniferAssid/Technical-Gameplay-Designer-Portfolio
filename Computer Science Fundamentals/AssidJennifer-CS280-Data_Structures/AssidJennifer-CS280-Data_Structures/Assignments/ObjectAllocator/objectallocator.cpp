/*****************************************************************//**
 * \file   ObjectAllocator.cpp
 * \author(s) Jennifer Assid
 *
 * \date   1/21/2022
 *********************************************************************/

#include "ObjectAllocator.h"
#include <string.h>

void ObjectAllocator::put_on_freelist(void* Object)
{
    /* Check to make sure the object is valid */
    if (Object == NULL)
    {
        /* If not - throw exception and return out of function */
        throw OAException(OAException::E_NO_MEMORY, "Object was NULL");
        return;
    }

    /* Cast the object to a GenericObject */
    GenericObject* object = reinterpret_cast<GenericObject*>(Object);

    /* Add to the FreeList and correct the FreeList pointer */
    object->Next = FreeList_;
    FreeList_ = object;

    /* Update the statistics */
    Stats_->FreeObjects_++;
}

void ObjectAllocator::allocate_new_page(void)
{
    /* Check if max pages has been reached */
    if (Stats_->PagesInUse_ == Config_->MaxPages_)
    {
        /* If not, throw an exception and return out of function */
        throw OAException(OAException::E_NO_MEMORY, "Max amount of pages has been reached.");
        return;
    }

    /* Initialize the page variable */
    unsigned char* new_page = NULL;

    /* Allocate a new page given the information in config_ and stats_ */
    try
    {
        new_page = new unsigned char[Stats_->PageSize_] {0};
    }
    catch (const std::bad_alloc& e)
    {
        throw OAException(OAException::E_NO_MEMORY, "Allocation of new page failed\n");
        return;
    }

    /* Cast the new_page to a GenericObject */
    GenericObject* new_page_object = reinterpret_cast<GenericObject*>(new_page);

    /* Add to the PageList and correct that PageList pointer */
    new_page_object->Next = PageList_;
    PageList_ = new_page_object;

    /* Update the statistics */
    Stats_->PagesInUse_++;

    /* Set the left alignment bits to their necessary values */
    if (Config_->DebugOn_ && Config_->LeftAlignSize_ != 0)
    {
        memset(new_page + sizeof(void*), ALIGN_PATTERN, sizeof(unsigned char) * Config_->LeftAlignSize_);
    }

    /* Add the objects in the new page to the FreeList_ */
    for (unsigned int i = 0; i < (Config_->ObjectsPerPage_); i++)
    {
        /* Get the modifiable pointer at the beginning of the current object */
        unsigned char* modify = new_page + PtrSize_ + ObjSize_ * i;

        /* Get the pointers to the surrounding padding */
        unsigned char* left_padding = modify - Config_->PadBytes_;
        unsigned char* right_padding = modify + Stats_->ObjectSize_;

        /* If Debug is on */
        if (Config_->DebugOn_)
        {
            /* Set through the object size and set the bytes to AA */
            memset(modify, UNALLOCATED_PATTERN, sizeof(unsigned char) * Stats_->ObjectSize_);

            /* Set the padding to its necessary information */
            memset(left_padding, PAD_PATTERN, sizeof(unsigned char) * Config_->PadBytes_);
            memset(right_padding, PAD_PATTERN, sizeof(unsigned char) * Config_->PadBytes_);

            /* Set the inter alignment as necessary */
            if (i != Config_->ObjectsPerPage_ - 1)
            {
                memset(right_padding + Config_->PadBytes_, ALIGN_PATTERN, sizeof(unsigned char) * Config_->InterAlignSize_);
            }
        }

        /* Cast the object so it can be placed on the freelist */
        GenericObject* object = reinterpret_cast<GenericObject*>(modify);
        /* Place the object on the free list */
        put_on_freelist(object);
    }
}

ObjectAllocator::ObjectAllocator(size_t ObjectSize, const OAConfig& config) : PageList_(NULL), FreeList_(NULL)
{
    /* Set the private configuration to the given configuration */
    try
    {
        Config_ = new OAConfig(config.UseCPPMemManager_,
            config.ObjectsPerPage_,
            config.MaxPages_,
            config.DebugOn_,
            config.PadBytes_,
            config.HBlockInfo_,
            config.Alignment_);
    }
    catch (const std::bad_alloc& e)
    {
        throw OAException(OAException::E_NO_MEMORY, "Allocation of Config_ failed\n");
        return;
    }

    /* Calculate the size of the pointer section (beginning of the page - before the first object) */
    PtrSize_ = sizeof(void*) + Config_->PadBytes_ + Config_->HBlockInfo_.size_;

    /* If there is alignment and the calculated memory size doesn't fall on the specified boundary */
    if (Config_->Alignment_ != 0 && PtrSize_ % Config_->Alignment_ != 0)
    {
        /* If the calculated memory block is larger than the alignment - subtract the alignment by the remainder and add the result of the size of the block */
        /* Else - subtract the memory size from alignment and add the difference to the memory size */
        Config_->LeftAlignSize_ = static_cast<unsigned int>((PtrSize_ > Config_->Alignment_) ?
            Config_->Alignment_ - (PtrSize_ % Config_->Alignment_) :
            Config_->Alignment_ - PtrSize_);
        PtrSize_ += Config_->LeftAlignSize_;
    }

    /* Calculate the size of the objects (with associated padding and headers) */
    ObjSize_ = ObjectSize + (Config_->PadBytes_ * 2) + Config_->HBlockInfo_.size_;

    /* If there is alignment and the calculated memory size doesn't fall on the specified boundary */
    if (Config_->Alignment_ != 0 && ObjSize_ % Config_->Alignment_ != 0)
    {
        /* If the memory block is larger than alignment - subtract the alignment by the remainder and add to the memory size */
        /* Else - subtract the memory size form the alignment and add the difference to the memory size */
        Config_->InterAlignSize_ = static_cast<unsigned int>((ObjSize_ > Config_->Alignment_) ?
            Config_->Alignment_ - (ObjSize_ % Config_->Alignment_) :
            Config_->Alignment_ - ObjSize_);
        ObjSize_ += Config_->InterAlignSize_;
    }

    /* Calculate the size of the page based off of the previous calcuations */
    size_t page_size = PtrSize_ + ((Config_->ObjectsPerPage_ - 1) * ObjSize_) + ObjectSize + Config_->PadBytes_;

    /* Attempt to allocate the statistics module */
    try
    {
        Stats_ = new OAStats();
    }
    catch (const std::bad_alloc& e)
    {
        throw OAException(OAException::E_NO_MEMORY, "Allocation of Stats_ failed\n");
        return;
    }

    /* Set the private statistics to the needed values */
    Stats_->ObjectSize_ = ObjectSize;
    Stats_->PageSize_ = page_size;

    /* Allocate the page */
    allocate_new_page();
}

ObjectAllocator::~ObjectAllocator()
{
    /* Go through and make sure that all memory is freed properly */
    delete Stats_;
    delete Config_;

    /* Free all of the memory associated with the pages */
    while (PageList_)
    {
        GenericObject* temp = PageList_;
        PageList_ = PageList_->Next;
        delete[] temp;
    }
}

void* ObjectAllocator::Allocate(const char* label)
{
    /* If the user wants to use the memory manager innate with CPP (not this one) - adjust statics as necessary and use base CPP functions */
    if (Config_->UseCPPMemManager_)
    {
        /* Adjust the statistics */
        Stats_->FreeObjects_--;
        Stats_->ObjectsInUse_++;
        if (Stats_->ObjectsInUse_ > Stats_->MostObjects_) Stats_->MostObjects_ = Stats_->ObjectsInUse_;
        Stats_->Allocations_++;
        return new GenericObject();
    }

    /* Check if the FreeList_ is empty */
    if (Stats_->FreeObjects_ == 0)
    {
        /* Try allocating a new page */
        try
        {
            allocate_new_page();
        }
        catch (const std::exception& e)
        {
            throw OAException(OAException::E_NO_MEMORY, "New page allocation failed.\n");
            return NULL;
        }

        /* If there are no new free objects - throw expection and return out of function */
        if (Stats_->FreeObjects_ == 0)
        {
            throw OAException(OAException::E_NO_MEMORY, "No available memory was found.\n");
            return NULL;
        }

    }

    /* Store the top of the free list in a local variable */
    GenericObject* free_block = FreeList_;

    /* Detach the head of the list and reassemble */
    FreeList_ = FreeList_->Next;

    /* Adjust the statistics */
    Stats_->FreeObjects_--;
    Stats_->ObjectsInUse_++;
    if (Stats_->ObjectsInUse_ > Stats_->MostObjects_) Stats_->MostObjects_ = Stats_->ObjectsInUse_;
    Stats_->Allocations_++;

    /* Change the pointer into something we can modify */
    unsigned char* modify = reinterpret_cast<unsigned char*>(free_block);

    /* If the Debug is set to true, set the values of the object to 0xBB */
    if (Config_->DebugOn_)
    {
        memset(modify, ALLOCATED_PATTERN, sizeof(unsigned char) * Stats_->ObjectSize_);
    }

    /* Move the pointer to the start of the header */
    modify -= Config_->PadBytes_ + Config_->HBlockInfo_.size_;

    /* Adjust logic based off the type of header being used */
    if (Config_->HBlockInfo_.type_ == OAConfig::HBLOCK_TYPE::hbBasic)
    {
        /* Convert the header pointer to its data */
        unsigned char* basic = (modify + sizeof(unsigned));
        /* Notate that the object is in use */
        *basic |= (1 << 0);
        /* Update the allocation number on the object */
        unsigned int* alloc = reinterpret_cast<unsigned int*>(modify);
        *alloc = Stats_->Allocations_;
    }
    else if (Config_->HBlockInfo_.type_ == OAConfig::HBLOCK_TYPE::hbExtended)
    {
        /* Update the number of times the object has been allocated */
        unsigned short* use_counter = reinterpret_cast<unsigned short*>(modify + Config_->HBlockInfo_.additional_);
        unsigned short temp = *use_counter;
        temp = static_cast<unsigned short>(temp + 1);
        *use_counter = temp;
        /* Update the allocation number on the object*/
        unsigned int* alloc_counter = reinterpret_cast<unsigned int*>(modify + Config_->HBlockInfo_.additional_ + sizeof(unsigned short));
        *alloc_counter = Stats_->Allocations_;
        /* Notate that the block of memory is in use */
        unsigned char* flag = (modify + Config_->HBlockInfo_.additional_ + sizeof(unsigned short) + sizeof(unsigned int));
        *flag = 1;
    }
    else if (Config_->HBlockInfo_.type_ == OAConfig::HBLOCK_TYPE::hbExternal)
    {
        /* Covert pointer to form of data that can be modified*/
        *reinterpret_cast<MemBlockInfo**>(modify) = new MemBlockInfo();
        /* Notate that the block of memory is in use */
        (*reinterpret_cast<MemBlockInfo**>(modify))->in_use = true;
        /* Update the allocation number of the block of memory */
        (*reinterpret_cast<MemBlockInfo**>(modify))->alloc_num = Stats_->Allocations_;
        /* Adjust the label of the header */
        (*reinterpret_cast<MemBlockInfo**>(modify))->label = new char[128];
        if (label)
        {
            memcpy((*reinterpret_cast<MemBlockInfo**>(modify))->label, label, sizeof(char) * 128);
        }
    }

    /* Return the result */
    return free_block;
}

void ObjectAllocator::Free(void* Object)
{
    /* Check to see if the object is valid */
    if (Object == NULL)
    {
        /* If not, throw an exception and return out of function */
        throw OAException(OAException::E_NO_MEMORY, "Invalid object; cannot be freed.\n");
        return;
    }

    /* If the user wants to use the innate memory manager for CPP (not this one), update the statistics and use CPP functions */
    if (Config_->UseCPPMemManager_)
    {
        /* Update the statistics */
        Stats_->ObjectsInUse_--;
        Stats_->Deallocations_++;
        delete reinterpret_cast<GenericObject*>(Object);
        return;
    }

    /* Get the head of FreeList into a modifable pointer */
    GenericObject* free_list = FreeList_;

    /* Iterate thorugh the free list to see if the object has been freed */
    for (unsigned int i = 0; i < Stats_->FreeObjects_; i++)
    {
        /* If the obejct has been free - throw an exception and return out of function */
        if (Object == free_list)
        {
            throw OAException(OAException::E_MULTIPLE_FREE, "Memory has already been freed.\n");
            return;
        }

        /* Continue iteration */
        free_list = free_list->Next;
    }

    /* Cast the pointer to something modifiable */
    unsigned char* modify = reinterpret_cast<unsigned char*>(Object);

    /* Initatie the check to see if the memory given is on a known boundary line */
    bool onBoundary = false;

    /* Get a stepper for the PageList */
    GenericObject* page_list = PageList_;

    /* Iterate through the pages */
    while (page_list)
    {
        /* Get the modifable page pointer */
        unsigned char* page = reinterpret_cast<unsigned char*>(page_list) + PtrSize_;

        /* Check to see if the object is on the page */
        if (modify >= page && modify <= reinterpret_cast<unsigned char*>(page_list) + Stats_->PageSize_)
        {
            /* Determine the distance between the current object and the beginning of the page */
            long distance = modify - page;

            /* If the object is on a boundary - update bool and break out of iteration */
            if (distance % ObjSize_ == 0)
            {
                onBoundary = true;
                break;
            }
        }

        /* Go to the next page */
        page_list = page_list->Next;
    }

    /* If the object is not on a boundary - throw an exception and return out of function */
    if (!onBoundary)
    {
        throw OAException(OAException::E_BAD_BOUNDARY, "");
        return;
    }

    /* Check to see if the memory is corrupted */

    /* Get the pointers to the surrounding padding */
    unsigned char* left_padding = modify - Config_->PadBytes_;
    unsigned char* right_padding = modify + Stats_->ObjectSize_;

    /* Iterate through the paddings */
    for (unsigned int i = 0; i < Config_->PadBytes_; i++)
    {
        /* If either padding has been overwritten with data - throw an exception and return out of the function */
        if (*(left_padding + i) != PAD_PATTERN || *(right_padding + i) != PAD_PATTERN)
        {
            throw OAException(OAException::E_CORRUPTED_BLOCK, "The memory has overrun the padding.\n");
            return;
        }
    }

    /* "Free" the memory */
    if (Config_->DebugOn_)
    {
        memset(modify, FREED_PATTERN, sizeof(unsigned char) * Stats_->ObjectSize_);
    }
    else
    {
        memset(modify, 0x00, sizeof(unsigned char) * Stats_->ObjectSize_);
    }

    /* Move the pointer to the start of the header */
    modify -= Config_->PadBytes_ + Config_->HBlockInfo_.size_;

    /* Adjust the logic based off of the time of header being used */
    if (Config_->HBlockInfo_.type_ == OAConfig::HBLOCK_TYPE::hbBasic)
    {
        /* Reset the number of allocations and flag */
        unsigned int* alloc = reinterpret_cast<unsigned int*>(modify);
        *alloc = 0;
        unsigned char* basic = (modify + sizeof(unsigned));
        *basic = 0;
    }
    else if (Config_->HBlockInfo_.type_ == OAConfig::HBLOCK_TYPE::hbExtended)
    {
        /* Reset the number of allocations and flag */
        unsigned int* alloc_counter = reinterpret_cast<unsigned int*>(modify + Config_->HBlockInfo_.additional_ + sizeof(unsigned short));
        *alloc_counter = 0;
        unsigned char* flag = (modify + Config_->HBlockInfo_.additional_ + sizeof(unsigned short) + sizeof(unsigned int));
        *flag = 0;
    }
    else if (Config_->HBlockInfo_.type_ == OAConfig::HBLOCK_TYPE::hbExternal)
    {
        /* Cast the pointer to a data type that can be modified */
        (*reinterpret_cast<MemBlockInfo**>(modify))->in_use = 0;
        /* Reset the allocation data */
        (*reinterpret_cast<MemBlockInfo**>(modify))->alloc_num = 0;
        /* Free the memory associated with the header label */
        delete[](*reinterpret_cast<MemBlockInfo**>(modify))->label;
        (*reinterpret_cast<MemBlockInfo**>(modify))->label = NULL;
        /* Delete the header */
        delete* reinterpret_cast<MemBlockInfo**>(modify);
        /* Reset the memory block */
        memset(modify, 0, sizeof(void*));

    }

    /* Update the statistics */
    Stats_->ObjectsInUse_--;
    Stats_->Deallocations_++;

    /* Cast pointer to something we can compare */
    GenericObject* object = reinterpret_cast<GenericObject*>(Object);

    /* Add the object to the FreeList_ */
    put_on_freelist(object);
}

unsigned ObjectAllocator::DumpMemoryInUse(DUMPCALLBACK fn) const
{
    /* If no header is being used - return the default value and return */
    if (Config_->HBlockInfo_.type_ == OAConfig::HBLOCK_TYPE::hbNone)
    {
        /* Return the number of blocks in use by the client */
        return static_cast<unsigned>(Stats_->ObjectSize_);
    }

    /* Get the head of the page list */
    GenericObject* page = reinterpret_cast<GenericObject*>(PageList_);

    /* Iterate through the pages to determine if the block of data is being used */
    for (unsigned int i = 0; i < Stats_->PagesInUse_; i++)
    {
        /* Get the modifiable pointer of the beginning of the page - after the pointer */
        unsigned char* page_char = reinterpret_cast<unsigned char*>(page) + PtrSize_;

        /* Iterate through the objects on the page */
        for (unsigned int j = 0; j < Config_->ObjectsPerPage_; j++)
        {
            /* Determine the pointer of the current memory block and current header */
            unsigned char* block = page_char + ObjSize_ * j;
            unsigned char* header = block - (Config_->PadBytes_ + Config_->HBlockInfo_.size_);

            /* Adjust the logic based off of the type of header */
            if (Config_->HBlockInfo_.type_ == OAConfig::HBLOCK_TYPE::hbBasic)
            {
                /* Get the pointer to the flag */
                unsigned char* flag = header + sizeof(unsigned);

                /* If the flag is triggered - the block is in use - dump the information */
                if (*flag == 0x1)
                {
                    fn(block, ObjSize_ - Config_->HBlockInfo_.size_);
                }
            }
            else if (Config_->HBlockInfo_.type_ == OAConfig::HBLOCK_TYPE::hbExtended)
            {
                /* Get the pointer to the flag */
                unsigned char* flag = header + sizeof(unsigned int) + sizeof(unsigned short) + Config_->HBlockInfo_.additional_;

                /* If the flag is triggered - the block is in use - dump the information */
                if (*flag == 0x1)
                {
                    fn(block, ObjSize_ - Config_->HBlockInfo_.size_);
                }
            }
            else if (Config_->HBlockInfo_.type_ == OAConfig::HBLOCK_TYPE::hbExternal)
            {
                /* If the flag is triggered - the block is in use - dump the information */
                if ((*reinterpret_cast<MemBlockInfo**>(header)) && (*reinterpret_cast<MemBlockInfo**>(header))->in_use)
                {
                    fn(block, ObjSize_ - Config_->HBlockInfo_.size_);
                }
            }
        }

        /* Continue iteration */
        page = page->Next;
    }

    /* Return the number of blocks in use by the client */
    return static_cast<unsigned>(Stats_->ObjectsInUse_);
}

unsigned ObjectAllocator::ValidatePages(VALIDATECALLBACK fn) const
{
    /* If Debug is off or there aren't any pad bytes - return */
    if (Config_->DebugOn_ == false || Config_->PadBytes_ == 0) return 0;

    /* Initialize the corrupted counter */
    unsigned corrupted = 0;

    /* Get a stepper variable for the page list */
    GenericObject* page_list = PageList_;

    /* Iterate through the page list */
    for (unsigned int i = 0; i < Stats_->PagesInUse_; i++)
    {
        /* Get the modifiable pointer for the start of the object list  */
        unsigned char* page_start = reinterpret_cast<unsigned char*>(page_list) + PtrSize_;

        /* Go through the objects in the page */
        for (unsigned int j = 0; j < Config_->ObjectsPerPage_; j++)
        {
            /* Get the left and right padding of the object */
            unsigned char* left_pad = page_start + ObjSize_ * j - Config_->PadBytes_;
            unsigned char* right_pad = page_start + ObjSize_ * j + Stats_->ObjectSize_;

            /* Iterate through the padding */
            for (unsigned int k = 0; k < Config_->PadBytes_; k++)
            {
                /* If the paddings have been overwritten */
                if (*(left_pad + k) != PAD_PATTERN || *(right_pad + k) != PAD_PATTERN)
                {
                    /* Increment the statistics and call the dump function */
                    corrupted++;
                    fn(left_pad, Stats_->ObjectSize_);
                    break;
                }
            }
        }

        /* Go to the next page */
        page_list = page_list->Next;
    }

    /* Return number of corrupted blocks - TO DO */
    return corrupted;
}

unsigned ObjectAllocator::FreeEmptyPages(void)
{
    /* Initialize free page counter */
    unsigned int pages_freed = 0;

    /* Get the stepper for the PageList */
    GenericObject* page_list = PageList_;
    /* Get a prev pointer to link up the list later */
    GenericObject* page_prev = NULL;

    /* Iterate through the pages using the stepper */
    while (page_list)
    {
        /* Get the modifiable pointer for the current page (starting at the beginning of the first object */
        unsigned char* page_ptr = reinterpret_cast<unsigned char*>(page_list) + PtrSize_;

        /* Reset the freed object count */
        unsigned int freed_objects = 0;

        /* Iterate through the objects on the page */
        for (unsigned int i = 0; i < Config_->ObjectsPerPage_; i++)
        {
            /* Get the current object */
            unsigned char* curr_obj = page_ptr + ObjSize_ * i;

            /* Get the stepper for the FreeList */
            GenericObject* free_list = FreeList_;

            /* Iterate through the FreeList using the stepper */
            while (free_list)
            {
                /* Compare the current free list object to the current object on the page */
                if (free_list == reinterpret_cast<GenericObject*>(curr_obj))
                {
                    /* If they are the same object, increment the freed object counter */
                    freed_objects++;
                }

                /* Go to the next object on the free list */
                free_list = free_list->Next;
            }
        }

        /* Check to see if all the objects on the current page have been freed */
        if (freed_objects == Config_->ObjectsPerPage_)
        {
            /* Iterate through the objects on the page again */
            for (unsigned int i = 0; i < Config_->ObjectsPerPage_; i++)
            {
                /* Get the current object */
                unsigned char* curr_obj = page_ptr + ObjSize_ * i;

                /* Get the stepper for the FreeList */
                GenericObject* free_list = FreeList_;
                /* Create a prev pointer to link up the FreeList */
                GenericObject* free_prev = NULL;

                /* Iterate through the FreeList using the stepper */
                while (free_list)
                {
                    /* Compare the current free list object to the current object on the page */
                    if (free_list == reinterpret_cast<GenericObject*>(curr_obj))
                    {
                        /* Remove the object from the FreeList */
                        free_list = free_list->Next;

                        /* If the head of the list has been removed - update the necessary pointer */
                        if (free_prev == NULL)
                        {
                            FreeList_ = free_list;
                        }
                        else
                        {
                            free_prev->Next = free_list;
                        }

                        /* Update the statistics */
                        Stats_->FreeObjects_--;

                        /* Continue iterating through the list */
                        break;
                    }
                    else
                    {
                        /* Update the pointers */
                        free_prev = free_list;
                        free_list = free_list->Next;
                    }
                }
            }

            /* Delete the current page and link the page list back up */
            GenericObject* temp = page_list;
            page_list = page_list->Next;
            delete[] reinterpret_cast<unsigned char*>(temp);

            /* Link the list back up */
            if (page_prev == NULL)
            {
                PageList_ = page_list;
            }
            else
            {
                page_prev->Next = page_list;
            }

            /* Update the statistics */
            Stats_->PagesInUse_--;
            pages_freed++;

            /* Continue through the iteration */
            continue;
        }
        else
        {
            /* Update the pointers  */
            page_prev = page_list;
            page_list = page_list->Next;
        }
    }

    /* Return the number of pages that has been freed */
    return pages_freed;
}

bool ObjectAllocator::ImplementedExtraCredit(void)
{
    return true;
}

void ObjectAllocator::SetDebugState(bool State)
{
    Config_->DebugOn_ = State;
}

const void* ObjectAllocator::GetFreeList(void) const
{
    return FreeList_;
}

const void* ObjectAllocator::GetPageList(void) const
{
    return PageList_;
}

OAConfig ObjectAllocator::GetConfig(void) const
{
    return *Config_;
}

OAStats ObjectAllocator::GetStats(void) const
{
    return *Stats_;
}

// I DO NOT OWN THE FOLLOWING CODE - IT IS BASED OFF OF A DRIVER PROVIDED TO ALL OF THE STUDENTS
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>

using std::cout;
using std::endl;
using std::printf;

int SHOWADDRESS1 = 0;
int SHOWADDRESS2 = 0;
int SHOWREAL = 0;
int SHOW_EXCEPTIONS = 0;

#include "ObjectAllocator.h"
#include "PRNG.h"

struct Student {
    int Age;
    float GPA;
    long Year;
    long ID;
};

struct Employee {
    Employee *Next;
    char lastName[12];
    char firstName[12];
    float salary;
    int years;
};


ObjectAllocator *studentObjectMgr;
ObjectAllocator *employeeObjectMgr;

// Support functions
void PrintCounts( const ObjectAllocator *nm );
void PrintCounts2( const ObjectAllocator *nm );
void PrintConfig( const ObjectAllocator *nm );
void DumpPages( const ObjectAllocator *nm, unsigned width = 16 );
void DumpPagesEx( const ObjectAllocator *nm, unsigned width = 16 );

void DoStudents( unsigned padding = 0, bool printall = false );  // debug, padding=X
void DoEmployees( void );             // debug
void TestPadding( unsigned size );             // DoStudents(padding=16)
void TestBasicHeaderBlocks();         // debug, header
void TestCorruption( void );          // debug, padding=8
void DisableOA( void );               // cpp_mem_mgr
void TestLeak( void );                // debug
void TestValidate( void );            // debug, padding=8
void TestAlignment( void );           // debug, padding=2, align=8
void TestFreeEmptyPages1( void );     // debug, padding=2
void TestFreeEmptyPages2( void );     // debug, padding=2, header, align=16
void TestFreeEmptyPages3( void );     // debug, padding=6
void StressFreeChecking( void );      //
void Stress( bool UseNewDelete );     //

struct Person {
    char lastName[12];
    char firstName[12];
    float salary;
    int years;
};

struct Person PEOPLE[] = {
    {"Faith", "Ian", 80000, 10},
    {"Tufnel", "Nigel", 90000, 12},
    {"Savage", "Viv", 50000, 4},
    {"Shrimpton", "Mick", 50000, 4},
    {"Besser", "Joe", 40000, 1},
    {"Smalls", "Derek", 80000, 10},
    {"St.Hubbins", "David", 90000, 12},
    {"Fleckman", "Bobbi", 120000, 8},
    {"Eton-Hogg", "Denis", 250000, 22},
    {"Upham", "Denny", 60000, 5},
    {"McLochness", "Ross", 60000, 5},
    {"Pudding", "Ronnie", 50000, 2},
    {"Schindler", "Danny", 60000, 3},
    {"Pettibone", "Jeanine", 85000, 3},
    {"Fame", "Duke", 95000, 8},
    {"Fufkin", "Artie", 45000, 1},
    {"DiBergi", "Marty", 15000, 7},
    {"Floyd", "Pink", 25000, 6},
    {"Zeppelin", "Led", 35000, 3},
    {"Mason", "Nick", 15000, 7},
    {"Wright", "Richard", 17000, 9},
    {"Waters", "Roger", 10000, 3},
    {"Gilmore", "David", 19000, 5}
};

void FillEmployee( Employee& emp )
{
    static unsigned int index = 0;
    strcpy( emp.firstName, PEOPLE[index].firstName );
    strcpy( emp.lastName, PEOPLE[index].lastName );
    emp.salary = PEOPLE[index].salary;
    emp.years = PEOPLE[index].years;
    index++;
    if( index >= sizeof( PEOPLE ) / sizeof( *PEOPLE ) )
        index = 0;
}

void DumpCallback( const void *block, size_t actual_size )
{
    size_t size = actual_size;
    // limit to 16 bytes
    if( actual_size > 16 )
        size = 16;
    unsigned char *data = const_cast<unsigned char*>( static_cast<const unsigned char *>( block ) );
    if( SHOWADDRESS1 )
        printf( "Block at 0x%p, %u bytes long.\n", block, static_cast<unsigned>( actual_size ) );
    else
        printf( "Block at 0x00000000, %u bytes long.\n", static_cast<unsigned>( actual_size ) );
    // If we were passed a NULL pointer, do nothing
    if( !block )
        return;
    printf( " Data: <" );
    for( unsigned int i = 0; i < size; i++ ) {
        unsigned char c = *data++;
        if( c > 31 && c < 128 ) // printable range
            printf( "%c", c );
        else
            printf( " " );
    }
    printf( ">" );
    data = const_cast<unsigned char*>( static_cast<const unsigned char *>( block ) );
    for( unsigned int i = 0; i < size; i++ )
        printf( " %02X", static_cast<int>( *data++ ) );
    printf( "\n" );
}

void DumpCallback2( const void *, size_t )
{
}

void CheckAndDumpLeaks( const ObjectAllocator* oa )
{
    if( oa->GetStats().ObjectsInUse_ ) {
        printf( "Detected memory leaks!\n" );
        printf( "Dumping objects ->\n" );
        unsigned leaks = oa->DumpMemoryInUse( DumpCallback );
        printf( "Object dump complete. [%u]\n", leaks );
    } else
        printf( "No leaks detected.\n" );
}

void ValidateCallback( const void *block, size_t actual_size )
{
    if( SHOWADDRESS2 )
        printf( "Block at 0x%p, %u bytes long.\n", block, static_cast<unsigned>( actual_size ) );
    else
        printf( "Block at 0x00000000, %u bytes long.\n", static_cast<unsigned>( actual_size ) );
    if( !block )
        return;
}

//****************************************************************************************************
//****************************************************************************************************
int RandomInt( int low, int high )
{
    //return std::rand() % (high - low + 1) + low;
    return CS170::Utils::Random( low, high );
}

template <typename T>
void SwapT( T &a, T &b )
{
    T temp = a;
    a = b;
    b = temp;
}

template <typename T>
void Shuffle( T *array, unsigned count )
{
    for( unsigned int i = 0; i < count; i++ ) {
        int r = RandomInt( i, static_cast<int>( count ) - 1 );
        SwapT( array[i], array[r] );
    }
}

template <typename T>
void PrintArray( T *array, unsigned count )
{
    for( unsigned i = 0; i < count; i++ )
        std::cout << array[i] << std::endl;
}

const unsigned objects = 1024;
const unsigned pages = 1024;
const unsigned total = objects * pages;
void *ptrs[total];

#include <ctime>
void Stress( bool UseNewDelete )
{
    ObjectAllocator *oa;
    try {
        bool newdel = UseNewDelete;
        bool debug = false;
        unsigned padbytes = 0;
        OAConfig::HeaderBlockInfo header( OAConfig::hbNone );
        unsigned alignment = 0;
        OAConfig config( newdel, objects, pages, debug, padbytes, header, alignment );
        oa  = new ObjectAllocator( sizeof( Student ), config );
        for( unsigned i = 0; i < total; i++ ) {
            void *p = oa->Allocate();
            ptrs[i] = p;
        }
        Shuffle( ptrs, total );
        for( unsigned i = 0; i < total; i++ ) {
            oa->Free( ptrs[i] );
        }
        delete oa;
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown during construction in Stress."  << endl;
        return;
    }
}

void StressFreeChecking( const OAConfig::HeaderBlockInfo& header )
{
    unsigned objects;
    unsigned pages;
    objects = 1000;
    pages = 10;
    const unsigned total = objects * pages;
    ObjectAllocator *oa;
    char **ptrs = new char*[total];
    try {
        bool newdel = false;
        bool debug = true;
        unsigned padbytes = 0;
        unsigned alignment = 0;
        OAConfig config( newdel, objects, pages, debug, padbytes, header, alignment );
        oa  = new ObjectAllocator( sizeof( Student ), config );
        for( unsigned int i = 0; i < total; i++ ) {
            void *p = oa->Allocate();
            ptrs[i] = static_cast<char *>( p );
        }
        PrintConfig( oa );
        PrintCounts( oa );
        //DumpPagesEx(oa, 42);
        Shuffle( ptrs, total );
        for( unsigned int i = 0; i < total; i++ ) {
            oa->Free( ptrs[i] );
        }
        oa->DumpMemoryInUse( DumpCallback2 );
        delete oa;
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else {
            cout << "Exception thrown during StressFreeChecking."  << endl;
            fflush( stdout );
        }
        return;
    }
    delete [] ptrs;
}

void TestFreeEmptyPages1( void )
{
    if( !ObjectAllocator::ImplementedExtraCredit() )
        return;
    ObjectAllocator *oa;
    const int objects = 4;
    const int pages = 3;
    const int total = objects * pages;
    void *ptrs[total];
    try {
        bool newdel = false;
        bool debug = true;
        unsigned padbytes = 2;
        OAConfig::HeaderBlockInfo header( OAConfig::hbNone );
        unsigned alignment = 0;
        OAConfig config( newdel, objects, pages, debug, padbytes, header, alignment );
        oa  = new ObjectAllocator( sizeof( Student ), config );
        unsigned width = 32;
        for( int i = 0; i < total; i++ ) {
            void *p = oa->Allocate();
            ptrs[i] = p;
        }
        //****************************************************************************
        PrintConfig( oa );
        PrintCounts( oa );
        DumpPages( oa, width );
        for( int i = 0; i < objects; i++ )
            oa->Free( ptrs[i + 0] );
        for( int i = 0; i < objects; i++ )
            oa->Free( ptrs[i + 8] );
        //****************************************************************************
        PrintCounts( oa );
        DumpPages( oa, width );
        //****************************************************************************
        oa->FreeEmptyPages();
        PrintCounts( oa );
        DumpPages( oa, width );
        //****************************************************************************
        for( int i = 0; i < objects - 1; i++ )
            oa->Free( ptrs[i + 4] );
        oa->FreeEmptyPages();
        PrintCounts( oa );
        DumpPages( oa, width );
        //****************************************************************************
        oa->Free( ptrs[7] );
        oa->FreeEmptyPages();
        PrintCounts( oa );
        DumpPages( oa, width );
        //****************************************************************************
        printf( "\n\n" );
        void *p = oa->Allocate();
        oa->FreeEmptyPages();
        PrintCounts( oa );
        DumpPages( oa, width );
        oa->Free( p );
        //oa->FreeEmptyPages();
        PrintCounts( oa );
        DumpPages( oa, width );
        oa->FreeEmptyPages();
        PrintCounts( oa );
        DumpPages( oa, width );
        delete oa;
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown during TestFreeEmptyPages1."  << endl;
        return;
    }
}

void TestFreeEmptyPages2( void )
{
    if( !ObjectAllocator::ImplementedExtraCredit() )
        return;
    ObjectAllocator *oa;
    const int objects = 4;
    const int pages = 3;
    const int total = objects * pages;
    void *ptrs[total];
    try {
        bool newdel = false;
        bool debug = true;
        unsigned padbytes = 2;
        OAConfig::HeaderBlockInfo header( OAConfig::hbBasic );
        unsigned alignment = 16;
        OAConfig config( newdel, objects, pages, debug, padbytes, header, alignment );
        oa  = new ObjectAllocator( sizeof( Student ), config );
        unsigned width = 32;
        for( int i = 0; i < total; i++ ) {
            void *p = oa->Allocate();
            ptrs[i] = p;
        }
        //****************************************************************************
        PrintConfig( oa );
        PrintCounts( oa );
        DumpPages( oa, width );
        for( int i = 0; i < objects; i++ )
            oa->Free( ptrs[i + 0] );
        for( int i = 0; i < objects; i++ )
            oa->Free( ptrs[i + 8] );
        //****************************************************************************
        PrintCounts( oa );
        DumpPages( oa, width );
        //****************************************************************************
        oa->FreeEmptyPages();
        PrintCounts( oa );
        DumpPages( oa, width );
        //****************************************************************************
        for( int i = 0; i < objects - 1; i++ )
            oa->Free( ptrs[i + 4] );
        oa->FreeEmptyPages();
        PrintCounts( oa );
        DumpPages( oa, width );
        //****************************************************************************
        oa->Free( ptrs[7] );
        oa->FreeEmptyPages();
        PrintCounts( oa );
        DumpPages( oa, width );
        delete oa;
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown during TestFreeEmptyPages2."  << endl;
        return;
    }
}

void TestFreeEmptyPages3( void )
{
    if( !ObjectAllocator::ImplementedExtraCredit() )
        return;
    ObjectAllocator *oa;
    const int objects = 8;
    const int pages = 48;
    const int total = objects * pages;
    void *ptrs[total];
    try {
        bool newdel = false;
        bool debug = true;
        unsigned padbytes = 6;
        OAConfig::HeaderBlockInfo header( OAConfig::hbExtended, 9 );
        unsigned alignment = 0;
        OAConfig config( newdel, objects, pages, debug, padbytes, header, alignment );
        oa  = new ObjectAllocator( sizeof( Student ), config );
        unsigned width = 32;
        for( int i = 0; i < total; i++ ) {
            void *p = oa->Allocate();
            ptrs[i] = p;
        }
        PrintConfig( oa );
        PrintCounts( oa );
        //DumpPages(oa, width);
        Shuffle( ptrs, total );
        for( int i = 0; i < total - 5; i++ ) {
            oa->Free( ptrs[i] );
        }
        PrintCounts( oa );
        //DumpPages(oa, width);
        unsigned count = oa->FreeEmptyPages();
        PrintCounts( oa );
        DumpPages( oa, width );
        printf( "%i pages freed\n", count );
        delete oa;
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown during TestFreeEmptyPages3."  << endl;
        return;
    }
}

//****************************************************************************************************
//****************************************************************************************************
void TestBasicHeaderBlocks()
{
    ObjectAllocator *oa = 0;
    try {
        bool newdel = false;
        bool debug = true;
        unsigned padbytes = 2;
        OAConfig::HeaderBlockInfo header( OAConfig::hbBasic );
        unsigned alignment = 1;
        OAConfig config( newdel, 4, 2, debug, padbytes, header, alignment );
        oa = new ObjectAllocator( sizeof( Student ), config );
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown during construction in TestBasicHeaderBlocks."  << endl;
        return;
    }
    unsigned wrap = 32;
    PrintConfig( oa );
    PrintCounts( oa );
    DumpPages( oa, wrap );
    Student *pStudent1 = 0;
    try {
        pStudent1 = static_cast<Student *>( oa->Allocate() );
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Allocate in TestBasicHeaderBlocks."  << endl;
    } catch( ... ) {
        cout << "Unexpected exception thrown from Allocate in TestBasicHeaderBlocks."  << endl;
    }
    Student *pStudent2 = 0;
    try {
        pStudent2 = static_cast<Student *>( oa->Allocate() );
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Allocate in TestBasicHeaderBlocks."  << endl;
    } catch( ... ) {
        cout << "Unexpected exception thrown from Allocate in TestBasicHeaderBlocks."  << endl;
    }
    DumpPages( oa, wrap );
    try {
        oa->Free( pStudent1 );
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Free in TestBasicHeaderBlocks."  << endl;
    } catch( ... ) {
        cout << "Unexpected exception thrown from Free in TestBasicHeaderBlocks."  << endl;
    }
    DumpPages( oa, wrap );
    try {
        oa->Free( pStudent2 );
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Free in TestBasicHeaderBlocks."  << endl;
    } catch( ... ) {
        cout << "Unexpected exception thrown from Free in TestBasicHeaderBlocks."  << endl;
    }
    DumpPages( oa, wrap );
    try {
        pStudent1 = static_cast<Student *>( oa->Allocate() );
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Allocate in TestBasicHeaderBlocks."  << endl;
    } catch( ... ) {
        cout << "Unexpected exception thrown from Allocate in TestBasicHeaderBlocks."  << endl;
    }
    DumpPages( oa, wrap );
    delete oa;
}

//****************************************************************************************************
//****************************************************************************************************
void TestExtendedHeaderBlocks( void )
{
    ObjectAllocator *oa = 0;
    unsigned wrap = 32;
    unsigned size = 1;
    try {
        bool newdel = false;
        bool debug = true;
        unsigned padbytes = 2;
        OAConfig::HeaderBlockInfo header( OAConfig::hbExtended, size );
        unsigned alignment = 0;
        OAConfig config( newdel, 4, 2, debug, padbytes, header, alignment );
        oa = new ObjectAllocator( sizeof( Student ), config );
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown during construction in TestExtendedHeaderBlocks."  << endl;
        return;
    }
    PrintConfig( oa );
    PrintCounts( oa );
    DumpPages( oa, wrap );
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    Student *pStudent1 = 0;
    try {
        pStudent1 = static_cast<Student *>( oa->Allocate() );
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Allocate in TestExtendedHeaderBlocks."  << endl;
    } catch( ... ) {
        cout << "Unexpected exception thrown from Allocate in TestExtendedHeaderBlocks."  << endl;
    }
    DumpPages( oa, wrap );
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    Student *pStudent2 = 0;
    try {
        pStudent2 = static_cast<Student *>( oa->Allocate() );
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Allocate in TestExtendedHeaderBlocks."  << endl;
    } catch( ... ) {
        cout << "Unexpected exception thrown from Allocate in TestExtendedHeaderBlocks."  << endl;
    }
    DumpPages( oa, wrap );
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    try {
        oa->Free( pStudent1 );
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Free in TestExtendedHeaderBlocks."  << endl;
    } catch( ... ) {
        cout << "Unexpected exception thrown from Free in TestExtendedHeaderBlocks."  << endl;
    }
    DumpPages( oa, wrap );
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    try {
        oa->Free( pStudent2 );
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Free in TestExtendedHeaderBlocks."  << endl;
    } catch( ... ) {
        cout << "Unexpected exception thrown from Free in TestExtendedHeaderBlocks."  << endl;
    }
    DumpPages( oa, wrap );
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    try {
        pStudent1 = static_cast<Student *>( oa->Allocate() );
        //pStudent2 = static_cast<Student *>( oa->Allocate() );
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Allocate in TestExtendedHeaderBlocks."  << endl;
    } catch( ... ) {
        cout << "Unexpected exception thrown from Allocate in TestExtendedHeaderBlocks."  << endl;
    }
    DumpPages( oa, wrap );
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    try {
        for( int i = 0; i < 5; i++ ) {
            oa->Free( pStudent1 );
            pStudent1 = static_cast<Student *>( oa->Allocate() );
        }
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Allocate in TestExtendedHeaderBlocks."  << endl;
    } catch( ... ) {
        cout << "Unexpected exception thrown from Allocate in TestExtendedHeaderBlocks."  << endl;
    }
    DumpPages( oa, wrap );
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    try {
        pStudent2 = static_cast<Student *>( oa->Allocate() );
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Allocate in TestExtendedHeaderBlocks."  << endl;
    } catch( ... ) {
        cout << "Unexpected exception thrown from Allocate in TestExtendedHeaderBlocks."  << endl;
    }
    DumpPages( oa, wrap );
    delete oa;
}

//****************************************************************************************************
//****************************************************************************************************
void TestExternalHeaderBlocks()
{
    ObjectAllocator *oa = 0;
    unsigned wrap = 32;
    try {
        bool newdel = false;
        bool debug = true;
        unsigned padbytes = 2;
        OAConfig::HeaderBlockInfo header( OAConfig::hbExternal );
        unsigned alignment = 0;
        OAConfig config( newdel, 4, 1, debug, padbytes, header, alignment );
        oa = new ObjectAllocator( sizeof( Student ), config );
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown during construction in TestExternalHeaderBlocks."  << endl;
        return;
    }
    PrintConfig( oa );
    PrintCounts( oa );
    DumpPagesEx( oa, wrap );
    Student *pStudent1 = 0;
    try {
        pStudent1 = static_cast<Student *>( oa->Allocate( "First student" ) );
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Allocate in TestExternalHeaderBlocks."  << endl;
    } catch( ... ) {
        cout << "Unexpected exception thrown from Allocate in TestExternalHeaderBlocks."  << endl;
    }
    PrintCounts( oa );
    DumpPagesEx( oa, wrap );
    Student *pStudent2 = 0;
    try {
        pStudent2 = static_cast<Student *>( oa->Allocate( "Second student" ) );
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Allocate in TestExternalHeaderBlocks."  << endl;
    } catch( ... ) {
        cout << "Unexpected exception thrown from Allocate in TestExternalHeaderBlocks."  << endl;
    }
    PrintCounts( oa );
    DumpPagesEx( oa, wrap );
    try {
        oa->Free( pStudent1 );
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Free in TestExternalHeaderBlocks."  << endl;
    } catch( ... ) {
        cout << "Unexpected exception thrown from Free in TestExternalHeaderBlocks."  << endl;
    }
    PrintCounts( oa );
    DumpPagesEx( oa, wrap );
    try {
        oa->Free( pStudent2 );
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Free in TestExternalHeaderBlocks."  << endl;
    } catch( ... ) {
        cout << "Unexpected exception thrown from Free in TestExternalHeaderBlocks."  << endl;
    }
    PrintCounts( oa );
    DumpPagesEx( oa, wrap );
    delete oa;
}

//****************************************************************************************************
//****************************************************************************************************
void DoStudents( unsigned padding, bool printall )
{
    try {
        bool newdel = false;
        bool debug = true;
        unsigned padbytes = padding;
        OAConfig::HeaderBlockInfo header( OAConfig::hbNone );
        unsigned alignment = 0;
        OAConfig config( newdel, 4, 2, debug, padbytes, header, alignment );
        studentObjectMgr = new ObjectAllocator( sizeof( Student ), config );
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown during construction in DoStudents."  << endl;
        return;
    } catch( ... ) {
        cout << "Unexpected exception thrown from constructor in DoStudents."  << endl;
    }
    unsigned wrap = 32;
    PrintConfig( studentObjectMgr );
    PrintCounts( studentObjectMgr );
    DumpPages( studentObjectMgr, wrap );
    Student *pStudent1 = 0, *pStudent2 = 0, *pStudent3 = 0;
    try {
        pStudent1 = static_cast<Student *>( studentObjectMgr->Allocate() );
        PrintCounts( studentObjectMgr );
        if( printall )
            DumpPages( studentObjectMgr, wrap );
        pStudent2 = static_cast<Student *>( studentObjectMgr->Allocate() );
        PrintCounts( studentObjectMgr );
        if( printall )
            DumpPages( studentObjectMgr, wrap );
        pStudent3 = static_cast<Student *>( studentObjectMgr->Allocate() );
        PrintCounts( studentObjectMgr );
        if( printall )
            DumpPages( studentObjectMgr, wrap );
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Allocate in DoStudents."  << endl;
    } catch( ... ) {
        cout << "Unexpected exception thrown from Allocate in DoStudents."  << endl;
    }
    Student *pStudent4[6];
    for( int i = 0; i < 6; i++ ) {
        try {
            pStudent4[i] = static_cast<Student *>( studentObjectMgr->Allocate() );
            if( pStudent4[i] == 0 )
                break;
        } catch( const OAException& e ) {
            if( SHOW_EXCEPTIONS )
                cout << e.what() << endl;
            else
                cout << "Exception thrown from Allocate (2) in DoStudents."  << endl;
            break;
        } catch( ... ) {
            cout << "Unexpected exception thrown from Allocate (2) in DoStudents."  << endl;
        }
    }
    PrintCounts( studentObjectMgr );
    if( printall )
        DumpPages( studentObjectMgr, wrap );
    try {
        studentObjectMgr->Free( pStudent1 );
        PrintCounts( studentObjectMgr );
        if( printall )
            DumpPages( studentObjectMgr, wrap );
        studentObjectMgr->Free( pStudent2 );
        PrintCounts( studentObjectMgr );
        if( printall )
            DumpPages( studentObjectMgr, wrap );
        studentObjectMgr->Free( pStudent3 );
        PrintCounts( studentObjectMgr );
        if( printall )
            DumpPages( studentObjectMgr, wrap );
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Free in DoStudents."  << endl;
    } catch( ... ) {
        cout << "Unexpected exception thrown from Free in DoStudents."  << endl;
    }
    cout << "Most students in use: " << studentObjectMgr->GetStats().MostObjects_ << endl;
    delete studentObjectMgr;
}

//****************************************************************************************************
//****************************************************************************************************
void TestPadding( unsigned size )
{
    DoStudents( size );
}

//****************************************************************************************************
//****************************************************************************************************
void DoEmployees( void )
{
    try {
        bool newdel = false;
        bool debug = true;
        unsigned padbytes = 0;
        OAConfig::HeaderBlockInfo header( OAConfig::hbNone );
        unsigned alignment = 0;
        OAConfig config( newdel, 4, 2, debug, padbytes, header, alignment );
        employeeObjectMgr = new ObjectAllocator( sizeof( Employee ), config );
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown during construction in DoEmployees."  << endl;
        return;
    } catch( ... ) {
        cout << "Unexpected exception thrown during constructor in DoEmployees."  << endl;
    }
    PrintConfig( employeeObjectMgr );
    PrintCounts( employeeObjectMgr );
    Employee *pEmployee1 = 0, *pEmployee2 = 0, *pEmployee3 = 0;
    try {
        pEmployee1 = static_cast<Employee *>( employeeObjectMgr->Allocate() );
        PrintCounts( employeeObjectMgr );
        pEmployee2 = static_cast<Employee *>( employeeObjectMgr->Allocate() );
        PrintCounts( employeeObjectMgr );
        pEmployee3 = static_cast<Employee *>( employeeObjectMgr->Allocate() );
        PrintCounts( employeeObjectMgr );
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Allocate in DoEmployees."  << endl;
    } catch( ... ) {
        cout << "Unexpected exception thrown from Allocate in DoEmployees."  << endl;
    }
    Employee *pEmployee4[7];
    for( int i = 0; i < 7; i++ ) {
        try {
            // Need to use /EHa, not /EHs
            // ******************************************* Puts &employeeObjMgr in ecx
            pEmployee4[i] = static_cast<Employee *>( employeeObjectMgr->Allocate() );
        } catch( const OAException& e ) {
            if( SHOW_EXCEPTIONS ) {
                // ******************** Hoses the ecx register containing &employeeObjMgr
                cout << e.what() << endl;
            } else
                cout << "Exception thrown from Allocate (2) in DoEmployees."  << endl;
            break;
        } catch( ... ) {
            cout << "Unexpected exception thrown from Allocate (2) in DoEmployees."  << endl;
        }
    }
    // ************************* blindly pushes ecx as if it still has &empObjMgr
    PrintCounts( employeeObjectMgr );
    try {
        employeeObjectMgr->Free( pEmployee1 );
        PrintCounts( employeeObjectMgr );
        employeeObjectMgr->Free( pEmployee2 );
        PrintCounts( employeeObjectMgr );
        employeeObjectMgr->Free( pEmployee3 );
        PrintCounts( employeeObjectMgr );
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Free in DoEmployees."  << endl;
    } catch( ... ) {
        cout << "Unexpected exception thrown from Free in DoEmployees."  << endl;
    }
    // Free an object twice
    try {
        employeeObjectMgr->Free( pEmployee1 );
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Free (Freeing object twice) in DoEmployees."  << endl;
    } catch( ... ) {
        cout << "Unexpected exception thrown from Free (Freeing object twice) in DoEmployees."  << endl;
    }
    // Free an invalid pointer (bad boundary condition)
    try {
        employeeObjectMgr->Free( reinterpret_cast<char *>( pEmployee4[0] + 4 ) );
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Free (Freeing address on bad boundary) in DoEmployees."  << endl;
    } catch( ... ) {
        cout << "Unexpected exception thrown from Free (Freeing address on bad boundary) in DoEmployees."  << endl;
    }
    cout << "Most employees in use: " << employeeObjectMgr->GetStats().MostObjects_<< endl;
    delete employeeObjectMgr;
}

//****************************************************************************************************
//****************************************************************************************************
void DisableOA( void )
{
    try {
        bool newdel = true;
        bool debug = false;
        unsigned padbytes = 0;
        OAConfig::HeaderBlockInfo header( OAConfig::hbNone );
        unsigned alignment = 0;
        OAConfig config( newdel, 4, 2, debug, padbytes, header, alignment );
        studentObjectMgr = new ObjectAllocator( sizeof( Student ), config );
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown during constructor in DisableOA."  << endl;
        return;
    } catch( ... ) {
        cout << "Unexpected exception thrown during constructor in DisableOA."  << endl;
    }
    PrintConfig( studentObjectMgr );
    PrintCounts2( studentObjectMgr );
    Student *pStudent1 = 0, *pStudent2 = 0, *pStudent3 = 0;
    try {
        pStudent1 = static_cast<Student *>( studentObjectMgr->Allocate() );
        PrintCounts2( studentObjectMgr );
        pStudent2 = static_cast<Student *>( studentObjectMgr->Allocate() );
        PrintCounts2( studentObjectMgr );
        pStudent3 = static_cast<Student *>( studentObjectMgr->Allocate() );
        PrintCounts2( studentObjectMgr );
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Allocate in DisableOA."  << endl;
    } catch( ... ) {
        cout << "Unexpected exception thrown from Allocate in DisableOA."  << endl;
    }
    Student *pStudent4[6];
    for( int i = 0; i < 6; i++ ) {
        try {
            pStudent4[i] = static_cast<Student *>( studentObjectMgr->Allocate() );
        } catch( const OAException& e ) {
            if( SHOW_EXCEPTIONS )
                cout << e.what() << endl;
            else
                cout << "Exception thrown from Allocate (2) in DisableOA."  << endl;
            break;
        } catch( ... ) {
            cout << "Unexpected exception thrown from Allocate (2) in DisableOA."  << endl;
        }
    }
    PrintCounts2( studentObjectMgr );
    try {
        studentObjectMgr->Free( pStudent1 );
        PrintCounts2( studentObjectMgr );
        studentObjectMgr->Free( pStudent2 );
        PrintCounts2( studentObjectMgr );
        studentObjectMgr->Free( pStudent3 );
        PrintCounts2( studentObjectMgr );
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Free in DisableOA."  << endl;
    } catch( ... ) {
        cout << "Unexpected exception thrown from Free in DIsableOA."  << endl;
    }
    for( int i = 0; i < 6; i++ ) {
        try {
            studentObjectMgr->Free( pStudent4[i] );
        } catch( const OAException& e ) {
            if( SHOW_EXCEPTIONS )
                cout << e.what() << endl;
            else
                cout << "Exception thrown from Free in DisableOA."  << endl;
            break;
        } catch( ... ) {
            cout << "Unexpected exception thrown from Free in DisableOA."  << endl;
        }
    }
    PrintCounts2( studentObjectMgr );
    cout << "Most students in use: " << studentObjectMgr->GetStats().MostObjects_ << endl;
    delete studentObjectMgr;
}

//****************************************************************************************************
//****************************************************************************************************
void StressBadBoundary( void )
{
    cout << "\n============================== StressBadBoundary..." << endl;
    int Objects = 100000;
    int Pages = 1;
    ObjectAllocator *oa;
    int total = Objects * Pages;
    void **ptrs = new void *[total];
    std::cout << "Stressing: " << Pages << " pages with " << Objects << " objects per page\n";
    try {
        bool newdel = false;
        bool debug = true;
        unsigned padbytes = 2;
        unsigned alignment = 0;
        OAConfig::HeaderBlockInfo header( OAConfig::hbBasic );
        OAConfig config( newdel, Objects, Pages, debug, padbytes, header, alignment );
        oa  = new ObjectAllocator( sizeof( Student ), config );
        for( int i = 0; i < total; i++ ) {
            void *p = oa->Allocate();
            ptrs[i] = p;
        }
        for( int i = 0; i < 10000; i++ ) {
            try {
                char *pp = reinterpret_cast<char *>( ptrs[Objects - 1] ) + 4;
                oa->Free( pp );
            } catch( const OAException& ) {
            }
        }
        delete oa;
        delete [] ptrs;
    } catch( const OAException& ) {
#ifdef SHOW_EXCEPTIONS
        cout << e.what() << endl;
#else
        cout << "Exception thrown during construction for Stress."  << endl;
#endif
        return;
    } catch( ... ) {
        cout << "Unexpected exception thrown from Stress."  << endl;
    }
}

//****************************************************************************************************
//****************************************************************************************************
void TestCorruption( void )
{
    ObjectAllocator *oa = 0;
    unsigned char *p;
    unsigned padbytes = 8;
    unsigned i;
    unsigned wrap = 32;
    Student *pStudent1, *pStudent2 = 0;
    try {
        bool newdel = false;
        bool debug = true;
        //unsigned header = 0;
        //OAConfig::HBLOCK_TYPE header = OAConfig::hbNone;
        OAConfig::HeaderBlockInfo header( OAConfig::hbBasic );
        unsigned alignment = 0;
        OAConfig config( newdel, 4, 2, debug, padbytes, header, alignment );
        //OAConfig config(false, 4, 2, true, padbytes);
        oa = new ObjectAllocator( sizeof( Student ), config );
        pStudent1 = static_cast<Student *>( oa->Allocate() );
        pStudent2 = static_cast<Student *>( oa->Allocate() );
        PrintConfig( oa );
        PrintCounts( oa );
        DumpPages( oa, wrap );
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown during construction/allocation in TestCorruption."  << endl;
        return;
    }
    // corrupt left pad bytes
    p = const_cast<unsigned char*>( reinterpret_cast<const unsigned char *>( pStudent1 ) ) - padbytes;
    for( i = 0; i < padbytes - 2; i++ )
        *p++ = 0xFF;
    // corrupt right pad bytes
    p = const_cast<unsigned char*>( reinterpret_cast<const unsigned char *>( pStudent2 ) ) + sizeof( Student );
    for( i = 0; i < padbytes - 2; i++ )
        *p++ = 0xEE;
    try {
        oa->Free( pStudent1 );
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else {
            if( e.code() == e.E_CORRUPTED_BLOCK )
                cout << "Exception thrown from Free: E_CORRUPTED_BLOCK on left"  << endl;
            else
                cout << "****** Unknown OAException thrown from Free in TestCorruption. ******"  << endl;
        }
    } catch( ... ) {
        cout << "Unexpected exception thrown from Free in TestCorruption."  << endl;
    }
    try {
        oa->Free( pStudent2 );
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else {
            if( e.code() == e.E_CORRUPTED_BLOCK )
                cout << "Exception thrown from Free: E_CORRUPTED_BLOCK on right"  << endl;
            else
                cout << "****** Unknown OAException thrown from Free (2) in TestCorruption. ******"  << endl;
        }
    } catch( ... ) {
        cout << "Unexpected exception thrown from Free (2) in TestCorruption."  << endl;
    }
    delete oa;
}

//****************************************************************************************************
//****************************************************************************************************
void TestLeak( void )
{
    unsigned count = 0;
    Employee **pEmps = 0;
    try {
        bool newdel = false;
        bool debug = true;
        unsigned padbytes = 0;
        OAConfig::HeaderBlockInfo header( OAConfig::hbBasic );
        unsigned alignment = 0;
        OAConfig config( newdel, 4, 8, debug, padbytes, header, alignment );
        employeeObjectMgr = new ObjectAllocator( sizeof( Employee ), config );
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown during constructor in TestLeak."  << endl;
        return;
    } catch( ... ) {
        cout << "Unexpected exception thrown during constructor in TestLeak."  << endl;
    }
    PrintConfig( employeeObjectMgr );
    PrintCounts( employeeObjectMgr );
    count = employeeObjectMgr->GetConfig().MaxPages_ * employeeObjectMgr->GetConfig().ObjectsPerPage_;
    pEmps = new Employee *[count];
    for( unsigned i = 0; i < count; i++ ) {
        try {
            pEmps[i] = static_cast<Employee *>( employeeObjectMgr->Allocate() );
            FillEmployee( *pEmps[i] );
        } catch( const OAException& e ) {
            if( SHOW_EXCEPTIONS )
                cout << e.what() << endl;
            else
                cout << "Exception thrown from Allocate in TestLeak."  << endl;
            break;
        } catch( ... ) {
            cout << "Unexpected exception thrown from Allocate in TestLeak."  << endl;
        }
    }
    PrintCounts( employeeObjectMgr );
    try {
        for( unsigned i = 0; i < count; i += 2 )
            employeeObjectMgr->Free( pEmps[i] );
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Free in TestLeak."  << endl;
    } catch( ... ) {
        cout << "Unexpected exception thrown from Free in TestLeak."  << endl;
    }
    PrintCounts( employeeObjectMgr );
    cout << "Most employees in use: " << employeeObjectMgr->GetStats().MostObjects_<< endl;
    cout << "\nChecking for leaks...\n";
    CheckAndDumpLeaks( employeeObjectMgr );
    try {
        for( unsigned i = 1; i < count; i += 2 )
            employeeObjectMgr->Free( pEmps[i] );
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown from Free (2) in TestLeak."  << endl;
    } catch( ... ) {
        cout << "Unexpected exception thrown from Free (2) in TestLeak."  << endl;
    }
    cout << "\nChecking for leaks...\n";
    CheckAndDumpLeaks( employeeObjectMgr );
    delete employeeObjectMgr;
    delete [] pEmps;
}


//****************************************************************************************************
//****************************************************************************************************
void TestValidate( void )
{
    ObjectAllocator *oa = 0;
    unsigned char *p;
    unsigned i, padbytes = 8;
    unsigned wrap = 32;
    Student *pStudent1 = 0, *pStudent2 = 0, *pStudent7 = 0;
    try {
        bool newdel = false;
        bool debug = true;
        OAConfig::HeaderBlockInfo header( OAConfig::hbBasic );
        unsigned alignment = 0;
        OAConfig config( newdel, 4, 2, debug, padbytes, header, alignment );
        oa = new ObjectAllocator( sizeof( Student ), config );
        pStudent1 = static_cast<Student *>( oa->Allocate() );
        pStudent2 = static_cast<Student *>( oa->Allocate() );
        oa->Allocate(); // 3
        oa->Allocate(); // 4
        oa->Allocate(); // 5
        oa->Allocate(); // 6
        pStudent7 = static_cast<Student *>( oa->Allocate() );
        PrintConfig( oa );
        PrintCounts( oa );
        DumpPages( oa, wrap );
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown during construction/allocation in TestValidate."  << endl;
        return;
    }
    try {
        // Validate a good heap
        unsigned count = oa->ValidatePages( ValidateCallback );
        cout << "Number of corruptions: " << count << endl << endl;
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else {
            if( e.code() == e.E_CORRUPTED_BLOCK )
                cout << "Exception thrown from Free: E_CORRUPTED_BLOCK on left"  << endl;
            else
                cout << "****** Unknown OAException thrown from Free in TestValidate. ******"  << endl;
        }
    } catch( ... ) {
        cout << "Unexpected exception thrown from Free in TestValidate."  << endl;
    }
    // corrupt left pad bytes of 1
    p = const_cast<unsigned char*>( reinterpret_cast<const unsigned char *>( pStudent1 ) ) - padbytes;
    for( i = 0; i < padbytes - 2; i++ )
        *p++ = 0xFF;
    // corrupt right pad bytes of 2
    p = const_cast<unsigned char*>( reinterpret_cast<const unsigned char *>( pStudent2 ) ) + sizeof( Student );
    for( i = 0; i < padbytes - 2; i++ )
        *p++ = 0xEE;
    PrintCounts( oa );
    DumpPages( oa, wrap );
    try {
        // Validate a corrupted heap
        unsigned count = oa->ValidatePages( ValidateCallback );
        cout << "Number of corruptions: " << count << endl << endl;
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else {
            if( e.code() == e.E_CORRUPTED_BLOCK )
                cout << "Exception thrown from Free: E_CORRUPTED_BLOCK on left"  << endl;
            else
                cout << "****** Unknown OAException thrown from Free (2) in TestValidate. ******"  << endl;
        }
    } catch( ... ) {
        cout << "Unexpected exception thrown from Free (2) in TestValidate."  << endl;
    }
    // corrupt left pad bytes of 7
    p = const_cast<unsigned char*>( reinterpret_cast<const unsigned char *>( pStudent7 ) ) - padbytes;
    for( i = 0; i < padbytes - 2; i++ )
        *p++ = 0xFF;
    // corrupt right pad bytes of 7
    p = const_cast<unsigned char*>( reinterpret_cast<const unsigned char *>( pStudent7 ) ) + sizeof( Student );
    for( i = 0; i < padbytes - 2; i++ )
        *p++ = 0xEE;
    PrintCounts( oa );
    DumpPages( oa, wrap );
    try {
        // Validate a corrupted heap
        unsigned count = oa->ValidatePages( ValidateCallback );
        cout << "Number of corruptions: " << count << endl << endl;
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else {
            if( e.code() == e.E_CORRUPTED_BLOCK )
                cout << "Exception thrown from Free: E_CORRUPTED_BLOCK on left"  << endl;
            else
                cout << "****** Unknown OAException thrown from Free (3) in TestValidate. ******"  << endl;
        }
    } catch( ... ) {
        cout << "Unexpected exception thrown from Free (3) in TestValidate."  << endl;
    }
    if( oa )
        delete oa;
}

//****************************************************************************************************
//****************************************************************************************************
void TestAlignment( void )
{
    if( !ObjectAllocator::ImplementedExtraCredit() )
        return;
    ObjectAllocator *oa = 0;
    try {
        unsigned wrap = 32;
        bool newdel = false;
        bool debug = true;
        unsigned padbytes = 2;
        //unsigned header = 0;
        OAConfig::HeaderBlockInfo header( OAConfig::hbBasic );
        unsigned alignment = 8;
        OAConfig config( newdel, 3, 2, debug, padbytes, header, alignment );
        if( !oa )
            oa = new ObjectAllocator( sizeof( Student ), config );
        PrintConfig( oa );
        PrintCounts( oa );
        DumpPages( oa, wrap );
        oa->Allocate(); // 1
        oa->Allocate(); // 2
        oa->Allocate(); // 3
        oa->Allocate(); // 4
        oa->Allocate(); // 5
        oa->Allocate(); // 6
        PrintCounts( oa );
        DumpPages( oa, wrap );
    } catch( const OAException& e ) {
        if( SHOW_EXCEPTIONS )
            cout << e.what() << endl;
        else
            cout << "Exception thrown during construction/allocation in TestAlignment."  << endl;
        return;
    }
    if( oa )
        delete oa;
}


void PrintCounts( const ObjectAllocator *nm )
{
    OAStats stats = nm->GetStats();
    cout << "Pages in use: " << stats.PagesInUse_;
    cout << ", Objects in use: " << stats.ObjectsInUse_;
    cout << ", Available objects: " << stats.FreeObjects_;
    cout << ", Allocs: " << stats.Allocations_;
    cout << ", Frees: " << stats.Deallocations_ << endl;
}

void PrintCounts2( const ObjectAllocator *nm )
{
    OAStats stats = nm->GetStats();
    cout << "Allocs: " << stats.Allocations_;
    cout << ", Frees: " << stats.Deallocations_ << endl;
}

void PrintConfig( const ObjectAllocator *oa )
{
    cout << "Object size = " << oa->GetStats().ObjectSize_;
    cout << ", Page size = " << oa->GetStats().PageSize_;
    cout << ", Pad bytes = " << oa->GetConfig().PadBytes_;
    cout << ", ObjectsPerPage = " << oa->GetConfig().ObjectsPerPage_;
    cout << ", MaxPages = " << oa->GetConfig().MaxPages_;
    cout << ", MaxObjects = " <<  oa->GetConfig().ObjectsPerPage_ * oa->GetConfig().MaxPages_;
    cout << endl;
    cout << "Alignment = " << oa->GetConfig().Alignment_;
    cout << ", LeftAlign = " << oa->GetConfig().LeftAlignSize_;
    cout << ", InterAlign = " << oa->GetConfig().InterAlignSize_;
    cout << ", HeaderBlocks = ";
    if( oa->GetConfig().HBlockInfo_.type_ == OAConfig::hbNone )
        cout << "None";
    else if( oa->GetConfig().HBlockInfo_.type_ == OAConfig::hbBasic )
        cout << "Basic";
    else if( oa->GetConfig().HBlockInfo_.type_ == OAConfig::hbExtended )
        cout << "Extended";
    else if( oa->GetConfig().HBlockInfo_.type_ == OAConfig::hbExternal )
        cout << "External";
    cout << ", Header size = " << oa->GetConfig().HBlockInfo_.size_;
    cout << endl;
}

void DumpPages( const ObjectAllocator *nm, unsigned width )
{
    const unsigned char *pages = static_cast<const unsigned char *>( nm->GetPageList() );
    const unsigned char *realpage = pages;
    size_t header_size = nm->GetConfig().HBlockInfo_.size_;
    while( pages ) {
        unsigned count = 0;
        if( SHOWREAL )
            printf( "%p\n", pages );
        else
            printf( "XXXXXXXX\n" );
        // print column header
        for( unsigned j = 0; j < width; j++ )
            printf( " %2i", j );
        printf( "\n" );
        // "Next page" pointer in the page
        if( SHOWREAL ) {
            for( unsigned i = 0; i < sizeof( void * ); i++, count++ )
                printf( " %02X", *pages++ );
        } else {
            for( unsigned j = 0; j < sizeof( void * ); pages++, count++, j++ )
                printf( " %s", "XX" );
        }
        // Left leading alignment bytes
        if( nm->GetConfig().Alignment_ > 1 ) {
            // leading alignment block (if any)
            for( unsigned j = 0; j < nm->GetConfig().LeftAlignSize_; count++, j++ ) {
                if( count >= width ) {
                    printf( "\n" );
                    count = 0;
                }
                printf( " %02X", *pages++ );
            }
        }
        // Dump each object and its associated info
        for( unsigned int i = 0; i < nm->GetConfig().ObjectsPerPage_ ; i++ ) {
            // inter-block alignment (not on first block)
            if( i > 0 ) {
                for( unsigned j = 0; j < nm->GetConfig().InterAlignSize_; count++, j++ ) {
                    if( count >= width ) {
                        printf( "\n" );
                        count = 0;
                    }
                    printf( " %02X", *pages++ );
                }
            }
            // header block bytes
            for( unsigned j = 0; j < header_size; count++, j++ ) {
                if( count >= width ) {
                    printf( "\n" );
                    count = 0;
                }
                printf( " %02X", *pages++ );
            }
            // left padding
            for( unsigned j = 0; j < nm->GetConfig().PadBytes_; count++, j++ ) {
                if( count >= width ) {
                    printf( "\n" );
                    count = 0;
                }
                printf( " %02X", *pages++ );
            }
            // possible next pointer (zero it out)
            for( unsigned j = 0; j < sizeof( void * ); count++, pages++, j++ ) {
                if( count >= width ) {
                    printf( "\n" );
                    count = 0;
                }
                if( SHOWREAL )
                    printf( " %02X", *pages );
                else
                    printf( " %s", "XX" );
            }
            // remaining bytes
            for( unsigned j = 0; j < nm->GetStats().ObjectSize_ - sizeof( void * ); count++, j++ ) {
                if( count >= width ) {
                    printf( "\n" );
                    count = 0;
                }
                printf( " %02X", *pages++ );
            }
            // right pad bytes
            for( unsigned j = 0; j < nm->GetConfig().PadBytes_; count++, j++ ) {
                if( count >= width ) {
                    printf( "\n" );
                    count = 0;
                }
                printf( " %02X", *pages++ );
            }
        }
        printf( "\n\n" );
        pages = reinterpret_cast<const unsigned char *>( ( reinterpret_cast<const GenericObject *>( realpage ) )->Next );
        realpage = pages;
    }
}

void DumpExternalHeaders( const ObjectAllocator *oa, const unsigned char *p )
{
    unsigned char *page = const_cast<unsigned char *>( p );
    unsigned padbytes = oa->GetConfig().PadBytes_;
    size_t header_size = oa->GetConfig().HBlockInfo_.size_;
    unsigned leftalign = oa->GetConfig().LeftAlignSize_;
    unsigned interalign = oa->GetConfig().InterAlignSize_;
    unsigned count = oa->GetConfig().ObjectsPerPage_;
    size_t offset = padbytes * 2 + header_size + interalign + oa->GetStats().ObjectSize_;
    page += sizeof( void * ); // the 'next' pointer
    page += leftalign;      // the left alignment bytes (if any)
    // We're now pointing at the beginning of the header block
    MemBlockInfo *pm = reinterpret_cast<MemBlockInfo *>( *reinterpret_cast<MemBlockInfo **>( page ) );
    cout << "  Label: " << ( pm && pm->label ? pm->label : "" ) << std::endl;
    cout << " In use: " << ( pm ? pm->in_use : 0 ) << std::endl;
    cout << "Alloc #: " << ( pm && pm->in_use ? pm->alloc_num : 0 ) << std::endl;
    for( unsigned i = 1; i < count; i++ ) {
        page += offset;
        MemBlockInfo *pm = reinterpret_cast<MemBlockInfo *>( *reinterpret_cast<MemBlockInfo **>( page ) );
        cout << "  Label: " << ( pm && pm->label ? pm->label : "" ) << std::endl;
        cout << " In use: " << ( pm ? pm->in_use : 0 ) << std::endl;
        cout << "Alloc #: " << ( pm && pm->in_use ? pm->alloc_num : 0 ) << std::endl;
    }
}

void DumpPagesEx( const ObjectAllocator *nm, unsigned width )
{
    const unsigned char *pages = static_cast<const unsigned char *>( nm->GetPageList() );
    const unsigned char *realpage = pages;
    size_t header_size = nm->GetConfig().HBlockInfo_.size_;
    while( pages ) {
        unsigned count = 0;
        if( SHOWREAL )
            printf( "%p\n", pages );
        else
            printf( "XXXXXXXX\n" );
        // print column header
        for( unsigned j = 0; j < width; j++ )
            printf( " %2i", j );
        printf( "\n" );
        if( SHOWREAL ) {
            for( unsigned i = 0; i < sizeof( void * ); i++, count++ )
                printf( " %02X", *pages++ );
        } else {
            for( unsigned j = 0; j < sizeof( void * ); pages++, count++, j++ )
                printf( " %s", "XX" );
        }
        // Left leading alignment bytes
        if( nm->GetConfig().Alignment_ > 1 ) {
            // leading alignment block (if any)
            for( unsigned j = 0; j < nm->GetConfig().LeftAlignSize_; count++, j++ ) {
                if( count >= width ) {
                    printf( "\n" );
                    count = 0;
                }
                printf( " %02X", *pages++ );
            }
        }
        for( unsigned int i = 0; i < nm->GetConfig().ObjectsPerPage_ ; i++ ) {
            // inter-block alignment (not on first block)
            if( i > 0 ) {
                for( unsigned j = 0; j < nm->GetConfig().InterAlignSize_; count++, j++ ) {
                    if( count >= width ) {
                        printf( "\n" );
                        count = 0;
                    }
                    printf( " %02X", *pages++ );
                }
            }
            // header block bytes
            for( unsigned j = 0; j < header_size; count++, j++, pages++ ) {
                if( count >= width ) {
                    printf( "\n" );
                    count = 0;
                }
                /*////////////////////======================================================================
                  In external mode, pointer address is embedded into the header memory
                /////////////////////=======================================================================*/
                if( SHOWREAL )
                    printf( " %02X", *pages );
                else
                    printf( " %s", "XX" );
            }
            // left padding
            for( unsigned j = 0; j < nm->GetConfig().PadBytes_; count++, j++ ) {
                if( count >= width ) {
                    printf( "\n" );
                    count = 0;
                }
                printf( " %02X", *pages++ );
            }
            // possible next pointer (zero it out)
            for( unsigned j = 0; j < sizeof( void * ); count++, pages++, j++ ) {
                if( count >= width ) {
                    printf( "\n" );
                    count = 0;
                }
                if( SHOWREAL )
                    printf( " %02X", *pages );
                else
                    printf( " %s", "XX" );
            }
            // remaining bytes
            for( unsigned j = 0; j < nm->GetStats().ObjectSize_ - sizeof( void * ); count++, j++ ) {
                if( count >= width ) {
                    printf( "\n" );
                    count = 0;
                }
                printf( " %02X", *pages++ );
            }
            // right pad bytes
            for( unsigned j = 0; j < nm->GetConfig().PadBytes_; count++, j++ ) {
                if( count >= width ) {
                    printf( "\n" );
                    count = 0;
                }
                printf( " %02X", *pages++ );
            }
        }
        printf( "\n" );
        DumpExternalHeaders( nm, realpage );
        printf( "\n" );
        pages = reinterpret_cast<const unsigned char *>( ( reinterpret_cast<const GenericObject *>( realpage ) )->Next );
        realpage = pages;
    }
}

void Test1( void )
{
    ObjectAllocator *oa;
    bool newdel = false;
    bool debug = false;
    unsigned padbytes = 0;
    OAConfig::HeaderBlockInfo header( OAConfig::hbExternal );
    unsigned alignment = 0;
    OAConfig config( newdel, 1, 1, debug, padbytes, header, alignment );
    oa  = new ObjectAllocator( sizeof( Student ), config );
    oa->Allocate();
    PrintConfig( oa );
    PrintCounts( oa );
    DumpPagesEx( oa, 32 );
    delete oa;
}

void DoStudents1()
{
    DoStudents( 0, false );
}

void DoStudents2()
{
    DoStudents( 0, true );
}

void TestPadding1()
{
    TestPadding( 6 );
}

void TestPadding2()
{
    TestPadding( 10 );
}

void StressFreeChecking1()
{
    StressFreeChecking( OAConfig::HeaderBlockInfo( OAConfig::hbBasic ) );
}

void StressFreeChecking2()
{
    StressFreeChecking( OAConfig::HeaderBlockInfo( OAConfig::hbExtended, 1 ) );
}

void StressFreeChecking3()
{
    StressFreeChecking( OAConfig::HeaderBlockInfo( OAConfig::hbExternal ) );
}

void Stress1()
{
    Stress( true );
}

void Stress2()
{
    Stress( false );
}



#include <fstream>
void Test20( void )
{
    // Makes a sentinel file so the scripts know what to check
    if( ObjectAllocator::ImplementedExtraCredit() ) {
        std::ofstream all( "all.txt" );
        all << "Implemented page freeing and alignment" << std::endl;
    }
}

//***********************************************************************
//***********************************************************************
//***********************************************************************

typedef void ( *TestFn )( void );

void ExecuteTest( TestFn fn, int maxwait = 1000, int safewait = 1000 )
{
    if( maxwait + safewait > 0 )
        fn();
}

//****************************************************************************************************
//****************************************************************************************************
void OA_DoStudents1()
{
    ExecuteTest(DoStudents1, 100, 100);
}


void OA_Tests(int test_num)
{
    struct TimedTest {
        void ( *test )( void );
        int maxwait;
        int safewait;
    };
    int max = 100;
    int safe = 100;
    int bigmax = 3000;
    int bigsafe = 1000 ;
    TimedTest Tests[] = {{DoStudents1,              max,    safe   }, // 1
        {DoStudents2,              max,    safe   }, // 2
        {DoEmployees,              max,    safe   }, // 3
        {TestPadding1,             max,    safe   }, // 4
        {TestPadding2,             max,    safe   }, // 5
        {TestBasicHeaderBlocks,    max,    safe   }, // 6
        {TestExtendedHeaderBlocks, max,    safe   }, // 7
        {TestExternalHeaderBlocks, max,    safe   }, // 8
        {TestCorruption,           max,    safe   }, // 9
        {DisableOA,                max,    safe   }, // 10
        {TestLeak,                 max,    safe   }, // 11
        {TestValidate,             max,    safe   }, // 12
        {StressFreeChecking1,      bigmax, bigsafe}, // 13
        {StressFreeChecking2,      bigmax, bigsafe}, // 14
        {StressFreeChecking3,      bigmax, bigsafe}, // 15
        {Stress1,                  bigmax, bigsafe}, // 16
        {Stress2,                  bigmax, bigsafe}, // 17
        {StressBadBoundary,        bigmax, bigsafe}, // 18
        {TestAlignment,            max,    safe   }, // 19 extra credit only
        {TestFreeEmptyPages1,      max,    safe   }, // 20 extra credit only
        {TestFreeEmptyPages2,      max,    safe   }, // 21 extra credit only
        {TestFreeEmptyPages3,      max,    safe   }, // 22 extra credit only
    };
    int num = sizeof( Tests ) / sizeof( *Tests );
    if(test_num == 30 ) {
        Test20(); // create sentinel file
    }
    if (test_num > num) return;
    if (test_num < 0) return;
}

