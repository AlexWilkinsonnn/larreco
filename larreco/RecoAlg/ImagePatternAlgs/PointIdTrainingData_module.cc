////////////////////////////////////////////////////////////////////////////////////////////////////
// Class:       PointIdAlg
// Author:      P.Plonski (pplonski86@gmail.com) and R.Sulej (Robert.Sulej@cern.ch), May 2016
//
// Training data for PointIdAlg
//
//      We use this to dump raw data for preparation of various classifiers.
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef PointIdTrainingData_Module
#define PointIdTrainingData_Module

#include "larcore/Geometry/Geometry.h"
#include "larcore/Geometry/GeometryCore.h"
#include "larcore/SimpleTypesAndConstants/geo_types.h"

#include "larreco/RecoAlg/ImagePatternAlgs/PointIdAlg/PointIdAlg.h"

// Framework includes
#include "art/Utilities/Exception.h"
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "fhiclcpp/ParameterSet.h"

// C++ Includes
#include <vector>
#include <string>
#include <cmath>
#include <fstream>

namespace nnet	 {

  class PointIdTrainingData : public art::EDAnalyzer
  {
  public:
 
    explicit PointIdTrainingData(fhicl::ParameterSet const& parameterSet);

    virtual void reconfigure(fhicl::ParameterSet const& parameterSet) override;

    virtual void analyze (const art::Event& event) override;

  private:

	std::string fOutTextFilePath;

	std::vector<int> fSelectedTPC;

	int fEvent;     ///< number of the event being processed
	int fRun;       ///< number of the run being processed
	int fSubRun;    ///< number of the sub-run being processed

	nnet::TrainingDataAlg fTrainingDataAlg;

	geo::GeometryCore const* fGeometry;
  };

  //-----------------------------------------------------------------------
  PointIdTrainingData::PointIdTrainingData(fhicl::ParameterSet const& parameterSet) : EDAnalyzer(parameterSet),
	fTrainingDataAlg(parameterSet.get< fhicl::ParameterSet >("TrainingDataAlg"))
  {
    fGeometry = &*(art::ServiceHandle<geo::Geometry>());
    reconfigure(parameterSet);
  }

  //-----------------------------------------------------------------------
  void PointIdTrainingData::reconfigure(fhicl::ParameterSet const& parameterSet)
  {
	fTrainingDataAlg.reconfigure(parameterSet.get< fhicl::ParameterSet >("TrainingDataAlg"));
	fOutTextFilePath = parameterSet.get< std::string >("OutTextFilePath");
	fSelectedTPC = parameterSet.get< std::vector<int> >("SelectedTPC");

	const size_t TPC_CNT = (size_t)fGeometry->NTPC(0);
	if (fSelectedTPC.empty())
	{
		for (size_t tpc = 0; tpc < TPC_CNT; ++tpc)
			fSelectedTPC.push_back(tpc);
	}
  }

  //-----------------------------------------------------------------------
  void PointIdTrainingData::analyze(const art::Event& event) 
  {
    fEvent  = event.id().event(); 
    fRun    = event.run();
    fSubRun = event.subRun();

	std::ostringstream os;
	os << "event_" << fEvent << "_run_" << fRun << "_subrun_" << fSubRun;

	std::cout << "analyze " << os.str() << std::endl;

	std::ofstream fout_raw, fout_deposit, fout_pdg;

	for (size_t i = 0; i < fSelectedTPC.size(); ++i)
	{
		std::ostringstream ss1;
		ss1 << fOutTextFilePath << "/raw_" << os.str() << "_tpc_" << fSelectedTPC[i];

		fout_raw.open(ss1.str() + ".raw");
		fout_deposit.open(ss1.str() + ".deposit");
		fout_pdg.open(ss1.str() + ".pdg");

		fTrainingDataAlg.setEventData(event, geo::kZ, fSelectedTPC[i], 0); // kZ is Collection

		for (size_t w = 0; w < fTrainingDataAlg.NWires(); ++w)
		{
			auto const & raw = fTrainingDataAlg.wireData(w);
			for (auto f : raw)
			{
				fout_raw << f << " ";
			}
			fout_raw << std::endl;

			auto const & edep = fTrainingDataAlg.wireEdep(w);
			for (auto f : edep)
			{
				fout_deposit << f << " ";
			}
			fout_deposit << std::endl;

			auto const & pdg = fTrainingDataAlg.wirePdg(w);
			for (auto f : pdg)
			{
				fout_pdg << f << " ";
			}
			fout_pdg << std::endl;
		}

		fout_raw.close();
		fout_deposit.close();
		fout_pdg.close();
	}

  } // Raw2DRegionID::analyze()

  DEFINE_ART_MODULE(PointIdTrainingData)

}

#endif // PointIdTrainingData_Module

