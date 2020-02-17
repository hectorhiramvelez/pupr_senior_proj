#pragma once

#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "rings_thread.hpp"
#include "grid.hpp"

#include <boost/thread/thread.hpp>

class sys
{
	private:
		rings_thread** th;
		size_t rings;
		double radius;
		size_t charges;
		double voltage;
		size_t iterations;
		size_t threads;

		grid* grd;
		size_t grid_width;
		size_t grid_height;
		size_t grid_depth;

		charge* ch;
		double distrib;
		size_t thread;

		/* Grid progress variables */
		boost::mutex grid_lock;
		volatile size_t grid_progress;
		void grid_progress_add(size_t value);
		size_t grid_progress_get();
		void grid_progress_reset();

		/* Program steps */
		void runIterations();
		void runLowestDistrib();
		void runGrid();
		static void runGridThread(sys* _this, size_t start, size_t size);
		void runResults();
		bool saveResults();
		static char* askSaveResults();


		char* resultParameters() const;
		char* resultRings() const;

		sys(const sys& S); /* Non-copyable */

	public:
		sys();
		~sys();

		void clear();
		
		/* Ring count */
		size_t getRings() const;
		size_t& getRings();
		void setRings(size_t rings);

		/* Ring radius */
		double getRadius() const;
		double& getRadius();
		void setRadius(double radius);

		/* Charges per ring */
		size_t getCharges() const;
		size_t& getCharges();
		void setCharges(size_t charges);

		/* Voltage */
		double getVoltage() const;
		double& getVoltage();
		void setVoltage(double voltage);

		/* Iterations */
		size_t getIterations() const;
		size_t& getIterations();
		void setIterations(size_t iterations);

		/* Threads */
		size_t getThreads() const;
		size_t& getThreads();
		void setThreads(size_t threads);

		/* Grid width */
		size_t getGridWidth() const;
		size_t& getGridWidth();
		void setGridWidth(size_t width);

		/* Grid height */
		size_t getGridHeight() const;
		size_t& getGridHeight();
		void setGridHeight(size_t height);

		/* Grid depth */
		size_t getGridDepth() const;
		size_t& getGridDepth();
		void setGridDepth(size_t depth);

		/* Combination of grid width x height x depth */
		void setGridSize(size_t width, size_t height, size_t depth);

		/* Configuration */
		static char* askConfig(bool save = false); /* NOTE: free() returned pointer after use */
		bool loadConfig(const char* _fname = "default");
		bool saveConfig(const char* _fname = "default");
		bool defaultConfig();

		/* Main function */
		bool main();
		bool run();

		/* After run functions */
		const rings_thread*const* getRingsArray() const;
		rings_thread** getRingsArray();

		const grid* getGrid() const;
		grid* getGrid();
};

#endif
