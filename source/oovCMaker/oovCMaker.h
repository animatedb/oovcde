// oovCMaker.h
// Created on: Jan 23, 2014
// \copyright 2014 DCBlaha.  Distributed under the GPL.

#include "Components.h"
#include "IncludeMap.h"
#include "Packages.h"
#include "Options.h"
#include "BuildConfigReader.h"


class CMaker
    {
    public:
        CMaker(char const * const projName, bool verbose):
            mProjectName(projName), mBuildPkgs(false), mVerbose(verbose)
            {}
        std::string getAnalysisPath() const
            {
            return mConfig.getAnalysisPath(BuildConfigAnalysis);
            }
        std::vector<std::string> getCompSources(char const * const compName);
        std::vector<std::string> getCompLibraries(char const * const compName);
        void makeTopMakelistsFile(char const * const destName);
        void makeTopLevelFiles(char const * const outDir);
        void makeToolchainFiles(char const * const outDir);
        void makeComponentFiles(bool writeToProject, char const * const outDir,
        	std::vector<std::string> const &compNames);
        void writeFile(char const * const destName, const std::string &str);

    public:
        std::string mProjectName;

        BuildConfigReader mConfig;
        ComponentTypesFile mCompTypes;
        BuildPackages mBuildPkgs;
        BuildOptions mBuildOptions;
        IncDirDependencyMapReader mIncMap;
        bool mVerbose;

        static void appendNames(std::vector<std::string> const &names, char delim,
            std::string &str);
	void addPackageDefines(char const * const pkgName, std::string &str);
	void makeDefineName(char const * const pkgName, OovString &defName);
        void makeToolchainFile(const char * const compilePath, char const * const destName);
        void makeTopInFile(char const * const destName);
        void makeTopVerInFile(char const * const destName);
        void makeComponentFile(char const * const compName,
            ComponentTypesFile::eCompTypes compType,
            std::vector<std::string> const &source, char const * const destName);
    };
