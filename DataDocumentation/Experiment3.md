# Definitions/Notes

  - Macro 1: refers to the first Macro you made for me (I believe it was
    1.4 specification guide) in excel

  - Macro 2: refers to the second macro you made for me using R

  - Of importance to note, I believe the most recent macro for Phase 1
    is Phase1\_Macro\_July29th.xlsm and V4 for Phase 2. These are the
    ones which should be updated. If this is not correct let me know.

  - All terminology should remain with the older macro specification
    guides
    
    1.  ## Files Included in package

  - macro\_1and2\_latest-stable – latest two stable versions of the old
    macros (I think, please correct me if wrong)

  - EXP2\_Mike\_Participant14.xml.janelle.csv – example of raw data file
    for a EXP2 participant

  - output\_headings\_3.0.xlsx – New headings (replaced all Nback with
    ATCint) and removed/added variables as specified below.

# Experiment 2 Information & Macro Guide

  - Experiment 2 was a replication of experiment 1, however, instead of
    being interrupted by an n-back task, participants were interrupted
    with an additional ATC task (i.e. an additional instance of the ATC
    simulator pact.exe was launched automatically for 27 seconds).
    
      - Therefore there the 3 interruption conditions are:
        
          - ATC Interruption (ATCint)
        
          - Blank Interruption (blank)
        
          - No Interruption

  - Consequently, all information in the macros should remain exactly
    the same **except** where there are references to the nback task
    (see section 4 of this document)

  - I estimate the easiest way to do this will be to modify macro 1 and
    2 (the excel one and the R one) to implement all reference of nback
    to ATCint.

# Looking up New Interruption

  - This following section will outline how to lookup the new
    interruption task

  - Below is an example of an output for a trial with the ATC
    Interruption (example attached in the macro package). The output for
    ATC interruption looks very similar to that of the blank
    interruption. The key difference is that on trials in which the
    external program interruption is used, the External Program column
    (Colum F) is filled with a value. These will always be filled with
    LaunchX.VBS (where X can vary from 1-5).
    
      - NOTE: LaunchX.VBS refers to a visual basic script which launched
        1 of 5 ATC scenarios into pact.exe
        
        ![](./media/image1.png)

  - The best way for me to describe the lookup process now is really to
    just write it in a programming-like language (I’ll call it
    MichaelScript – I hope this makes some sense, if not just let me
    know and I can clarify better):  
    ![](./media/image2.png)

# Data Outputted

  - All data outputted should be exactly the same as macro 1 and 2 with
    the following exceptions:
    
      - For macro 2 (the R one), I want to also include an additional 5
        columns which lists what specific Launch\[1:5\].VBS was done, in
        order (I don’t mind where they output, though currently are
        tacked on the end of the Macro 2 headings).
        
          - The corresponding headings would be ATC\_launchnum1 through
            to ATC\_launchnum5. Under ATC\_launchnum1 the first
            launchX.VBS value needs to be outputted, under
            ATC\_launchnum2, the second occurrence of a launchX.VBS
            needs to be outputted…. Etc.
    
      - I have removed all headings referencing Nback task scores. I.e.
        Section 6 of macro document 2.6 will be entirely omitted as this
        refers to getting the nback task scores which are no longer in
        the experiment.
    
      - I have renamed all instances of the word nback in the headings
        to ATC. Other than this (and exceptions above), all headings
        should remain the same.
