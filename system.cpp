#include "system.hpp"

#include "menu.hpp"
#include "prompt.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <limits>

#define config_suffix ".cfg" /* <-- Configuration */
#define data_suffix ".dat" /* <-- Grid data NOTE: Data is stored in a raw array of double */
#define info_suffix ".txt" /* <-- Results */

const unsigned int config_signature = *(reinterpret_cast<const unsigned int*>("ENRG"));

struct struct_config
{
	unsigned int signature;

	size_t rings;
	double radius;
	size_t charges;
	double voltage;
	size_t iterations;
	size_t threads;

	size_t grid_width;
	size_t grid_height;
	size_t grid_depth;
};

void sys::grid_progress_add(size_t value)
{
	autolock __lock(this->grid_lock);
	this->grid_progress += value;
}

size_t sys::grid_progress_get()
{
	autolock __lock(this->grid_lock);
	return this->grid_progress;
}

void sys::grid_progress_reset()
{
	autolock __lock(this->grid_lock);
	this->grid_progress = 0;
}

void sys::runIterations()
{
	bool finish;
	size_t i;
	unsigned short percent_cur, percent_next;

	this->th = new rings_thread*[this->threads];

	printf("Initializing threads"); fflush(stdout);
	i = 0;
	srand(time(0));
	while (i < this->getThreads())
	{
		this->th[i] = new rings_thread(this->getIterations() / this->getThreads(), this->getRings(), this->getCharges(), this->getRadius(), rand());
		printf("."); fflush(stdout);
		i++;
	}
	printf("done\n");

	percent_next = 5;
	printf("Running %ld iterations (%ld per thread for %ld threads)", (this->getIterations() / this->getThreads()) * this->getThreads(), this->getIterations() / this->getThreads(), this->getThreads()); fflush(stdout);
	finish = false;
	while (finish == false)
	{
		double total;
		boost::this_thread::sleep(boost::posix_time::milliseconds(50));

		i = 0; total = 0; finish = true;
		while (i < this->getThreads())
		{
			if (this->th[i]->finished() == false) finish = false;
			total += th[i]->getProgress();
			i++;
		}
		total /= this->getThreads() * 1.00;
		
		percent_cur = total * 100;
		while ((percent_cur > percent_next) && (percent_next < 100))
		{
			if ((percent_next % 25) == 0)
			{
				printf("%hd%s", percent_next, "%"); fflush(stdout);
			}
			else
			{
				printf("."); fflush(stdout);
			}
			percent_next += 5;
		}
	}
	printf("done\n");
}

void sys::runLowestDistrib()
{
	size_t i;

	i = 0;
	this->distrib = std::numeric_limits<double>::infinity();
	//printf("Thread statistics\n=================\n");
	while (i < this->getThreads())
	{
		if (this->th[i]->getDistrib() < this->distrib)
		{
			this->distrib = this->th[i]->getDistrib();
			this->thread = i;
			this->ch = this->th[i]->getCharges();
		}

		//printf("Thread %ld: %lf\n", i + 1, this->th[i]->getDistrib());
		i++;
	}
	//printf("\nLowest thread: %ld (%lf)\n-----------------\n", this->thread + 1, this->distrib);
}

void sys::runGrid()
{
	size_t i, grid_size, grid_slice, grid_missing, grid_progress;
	size_t percent_cur, percent_next;
	bool finished;
	boost::thread_group th;

	printf("Creating grid..."); fflush(stdout);
	this->grd = new grid(this->getRadius() * 2.25, this->getRadius() * 2.25, this->getRadius() * 2.25, this->getRadius() * -1.125, this->getRadius() * -1.125, this->getRadius() * -1.125, this->getGridWidth(), this->getGridHeight(), this->getGridDepth());
	this->grd->init();
	printf("done\n");

	grid_size = this->getGridWidth() * this->getGridHeight() * this->getGridDepth();
	grid_slice = grid_size / this->getThreads();
	grid_missing = grid_size - (grid_slice * this->getThreads());
	this->grid_progress_reset();

	i = 0;
	printf("Initializing threads"); fflush(stdout);
	while (i < this->getThreads())
	{
		th.add_thread(new boost::thread(this->runGridThread, this, grid_slice * i, grid_slice));
		printf("."); fflush(stdout);
		i++;
	}
	this->runGridThread(this, grid_slice * i, grid_missing); /* run the slack on itself */
	printf("done\n");

	printf("Calculating energy field (%ld threads)", this->getThreads()); fflush(stdout);
	finished = false; percent_next = 5;
	while (finished == false)
	{
		boost::this_thread::sleep(boost::posix_time::milliseconds(50));

		grid_progress = this->grid_progress_get();
		finished = grid_progress == grid_size;

		percent_cur = ((grid_progress * 1.00) / (grid_size * 1.00)) * 100;
		while ((percent_cur >= (percent_next + 0.05)) && (percent_next != 100))
		{
			if ((percent_next % 25) == 0)
			{
				printf("%lu%%", percent_next); fflush(stdout);
			}
			else
			{
				printf("."); fflush(stdout);
			}
			fflush(stdout);

			percent_next += 5;
		}
	}
	th.join_all();
	printf("done\n");

}

#ifdef this
#undef this /* Should not happen */
#endif
#define this _this
void sys::runGridThread(sys* this, size_t start, size_t size) /* <-- Intended to be a thread */
{
	size_t i, j, size2;
	vector3<double> pos;
	double value, volt;
	charge_s* ch;

	size2 = this->getRings() * this->getCharges();
	volt = this->getVoltage();
	ch = this->grd->getArray();

	i = 0;
	while (i < size)
	{
		pos = this->grd->getPosition(start + i);
		j = 0; value = 0;
		while (j < size2)
		{
			double mag;

			if ((mag = pos.difference(this->ch[j]).magnitude()) != 0)
			{
				value += (1.00 / mag) * volt;
			}
			else
			{
				value = std::numeric_limits<double>::infinity();
				//printf("Infinity hit\n");
				j = size2;
			}
			j++;
		}
		ch[start + i].setLoad(value);
		//this->grd->getCharge(start + i).setLoad(value);
		//printf("pos(%lf, %lf, %lf) = %lf\n", pos.getX(), pos.getY(), pos.getZ(), value);

		i++;

		if ((i & 255) == 0)
		{
			this->grid_progress_add(256);
		}
	}
	this->grid_progress_add(i % 256);
}
#undef this

void sys::runResults()
{
	char* tmp;

	printf("%s\n", tmp = this->resultParameters());
	free(tmp);

	printf("%s\n", tmp = this->resultRings());
	free(tmp);
}

bool sys::saveResults()
{
	FILE* fhandle;
	char* tmp;
	char* fname;
	char* fname2;
	charge_s* tmp2;

	if ((fname = this->askSaveResults()) == 0) return false;
	if ((fname2 = reinterpret_cast<char*>(malloc(sizeof(char) * BUFSIZ))) == 0)
	{
		printf("ERROR: Out of memory\n");
		free(fname);
		return false;
	}

	printf("Saving configuration to %s%s (you can load the configuration later with name %s)...", fname, config_suffix, fname); fflush(stdout);
	this->saveConfig(fname);
	printf("done\n");

	printf("Saving results information to %s%s...", fname, info_suffix); fflush(stdout);
	{
		sprintf(fname2, "%s%s", fname, info_suffix);
		if ((fhandle = fopen(fname2, "wb")) == 0)
		{
			printf("ERROR: Unable to open %s for output\n", fname2);
			free(fname2);
			free(fname);
			return false;
		}

		tmp = this->resultParameters();
		if (fwrite(tmp, strlen(tmp), 1, fhandle) != 1)
		{
			printf("ERROR: Unable to write to %s\n", fname2);
			fclose(fhandle);
			free(tmp);
			free(fname2);
			free(fname);
			return false;
		}
		free(tmp);

		tmp = this->resultRings();
		if (fwrite(tmp, strlen(tmp), 1, fhandle) != 1)
		{
			printf("ERROR: Unable to write to %s\n", fname2);
			fclose(fhandle);
			free(tmp);
			free(fname2);
			free(fname);
			return false;
		}
		free(tmp);

		fclose(fhandle);
	}
	printf("done\n");

	printf("Saving \"RAW\" grid data to %s%s (this may take a while)...", fname, data_suffix); fflush(stdout);
	{
		sprintf(fname2, "%s%s", fname, data_suffix);
		if ((fhandle = fopen(fname2, "wb")) == 0)
		{
			printf("ERROR: Unable to open %s for output\n", fname2);
			free(fname2);
			free(fname);
			return false;
		}

		tmp2 = this->grd->getArray();

		size_t i, size, w;
		i = 0; size = this->getGridWidth() * this->getGridHeight() * this->getGridDepth();
		while (i < size)
		{
			w = size - i;
			if (w > 64) w = 64;

			if (fwrite(tmp2, sizeof(charge_s), w, fhandle) != w)
			{
				printf("ERROR: Unable to write to %s\n", fname2);
				fclose(fhandle);
				free(fname2);
				free(fname);
				return false;
			}
			tmp2 = &tmp2[w];

			i += w;
		}		

		fclose(fhandle);
	}
	printf("done\n");

	free(fname2);
	free(fname);
	return true;
}

char* sys::askSaveResults()
{
	FILE* fhandle;
	char* msg;
	char* buffer;
	char* fname;

	if ((buffer = reinterpret_cast<char*>(malloc(sizeof(char) * BUFSIZ))) == 0) return 0;
	if ((msg = reinterpret_cast<char*>(malloc(sizeof(char) * BUFSIZ))) == 0)
	{
		free(buffer);
		return 0;
	}
	if ((fname = reinterpret_cast<char*>(malloc(sizeof(char) * BUFSIZ))) == 0)
	{
		free(msg);
		free(buffer);
		return 0;
	}

	if (sprintf(msg, "Filename prefix to save to (leave empty for \"output\")") == 0)
	{
		free(fname);
		free(msg);
		free(buffer);
		return 0;
	}

	buffer = prompt_string(msg, buffer, BUFSIZ); free(msg);
	if (strlen(buffer) == 0)
	{
		printf("No filename entered, assuming output\n");
		strcpy(buffer, "output");
	}

	sprintf(fname, "%s%s", buffer, config_suffix);
	if ((fhandle = fopen(fname, "r")) != 0)
	{
		if (prompt_yn("File already exist, save anyway?") == false)
		{
			printf("Save cancelled\n");
			free(buffer);
			return 0;
		}
		fclose(fhandle);
	}
	free(fname);

	return buffer;
}

char* sys::resultParameters() const
{
	char* buffer;
	char* ptr;

	if ((buffer = reinterpret_cast<char*>(malloc(sizeof(char) * BUFSIZ))) == 0) return 0;
	ptr = buffer;

	ptr = &ptr[sprintf(ptr, "Configuration Parameters\n========================\n")];

	ptr = &ptr[sprintf(ptr, "Rings: %ld\n", this->getRings())];
	ptr = &ptr[sprintf(ptr, "Ring Radius: %lf\n", this->getRadius())];
	ptr = &ptr[sprintf(ptr, "Charges per Ring: %ld\n", this->getCharges())];
	ptr = &ptr[sprintf(ptr, "Voltage: %lf V\n", this->getVoltage())];
	ptr = &ptr[sprintf(ptr, "Iterations: %ld\n", this->getIterations())];
	ptr = &ptr[sprintf(ptr, "Threads: %ld\n\n", this->getThreads())];

	ptr = &ptr[sprintf(ptr, "Iterations per Thread: %ld\n", this->getIterations() / this->getThreads())];
	ptr = &ptr[sprintf(ptr, "Total iterations: %ld\n", (this->getIterations() / this->getThreads()) * this->getThreads())];
	ptr = &ptr[sprintf(ptr, "Skipped iterations: %ld\n\n", this->getIterations() - ((this->getIterations() / this->getThreads()) * this->getThreads()))];

	ptr = &ptr[sprintf(ptr, "Grid size (Width): %ld\n", this->getGridWidth())];
	ptr = &ptr[sprintf(ptr, "Grid size (Height): %ld\n", this->getGridHeight())];
	ptr = &ptr[sprintf(ptr, "Grid size (Depth): %ld\n", this->getGridDepth())];

	ptr = &ptr[sprintf(ptr, "------------------------\n")];

	return buffer;
}

char* sys::resultRings() const
{
	char* buffer;
	char* ptr;
	size_t i, size;

	if ((buffer = reinterpret_cast<char*>(malloc(sizeof(char) * BUFSIZ))) == 0) return 0;
	ptr = buffer;

	ptr = &ptr[sprintf(ptr, "Ring charge positions\n=====================\n")];
	i = 0; size = this->getRings() * this->getCharges();
	while (i < size)
	{
		if ((i % this->getCharges()) == 0)
		{
			ptr = &ptr[sprintf(ptr, "Ring %ld:\n", (i / this->getCharges()) + 1)];
		}
		ptr = &ptr[sprintf(ptr, " --> Charge %ld coordinates: (%lf, %lf, %lf)\n", (i % this->getCharges()) + 1, this->ch[i].getX(), this->ch[i].getY(), this->ch[i].getZ())];
		i++;
	}
	ptr = &ptr[sprintf(ptr, "\nCharges distirubion: %lf\n", this->distrib)];
	ptr = &ptr[sprintf(ptr, "--------------------\n\n")];

	return buffer;
}

sys::sys(const sys& S)
{
	/* Freeze on purpose */
	if (&S == &S) while (true); /* Supress warnings */
	while(true);
}

sys::sys()
{
	this->th = 0;
	this->grd = 0;
	this->ch = 0;

	this->defaultConfig();
}

sys::~sys()
{
	/* TODO: sys::~sys() */
	this->clear();
}

void sys::clear()
{
	size_t i;
	if (this->th != 0)
	{
		i = 0;
		while (i < this->getThreads())
		{
			delete this->th[i];
			i++;
		}
		delete[] this->th;
		this->th = 0;
	}
	if (this->grd != 0)
	{
		delete this->grd;
		this->grd = 0;
	}
}

size_t sys::getRings() const
{
	return this->rings;
}

size_t& sys::getRings()
{
	return this->rings;
}

void sys::setRings(size_t rings)
{
	if (this->th != 0) return;
	this->rings = rings;
}

double sys::getRadius() const
{
	return this->radius;
}

double& sys::getRadius()
{
	return this->radius;
}

void sys::setRadius(double radius)
{
	if (this->th != 0) return;
	this->radius = radius;
}

size_t sys::getCharges() const
{
	return this->charges;
}

size_t& sys::getCharges()
{
	return this->charges;
}

void sys::setCharges(size_t charges)
{
	if (this->th != 0) return;
	this->charges = charges;
}

double sys::getVoltage() const
{
	return this->voltage;
}

double& sys::getVoltage()
{
	return this->voltage;
}

void sys::setVoltage(double voltage)
{
	if (this->th != 0) return;
	this->voltage = voltage;
}

size_t sys::getIterations() const
{
	return this->iterations;
}

size_t& sys::getIterations()
{
	return this->iterations;
}

void sys::setIterations(size_t iterations)
{
	if (this->th != 0) return;
	this->iterations = iterations;
}

size_t sys::getThreads() const
{
	return this->threads;
}

size_t& sys::getThreads()
{
	return this->threads;
}

void sys::setThreads(size_t threads)
{
	if (this->th != 0) return;
	if (threads < 1) threads = 1;
	this->threads = threads;
}

size_t sys::getGridWidth() const
{
	return grid_width;
}

size_t& sys::getGridWidth()
{
	return grid_width;
}

void sys::setGridWidth(size_t width)
{
	if (this->grd != 0) return;
	this->grid_width = width;
}

size_t sys::getGridHeight() const
{
	return grid_height;
}

size_t& sys::getGridHeight()
{
	return grid_height;
}

void sys::setGridHeight(size_t height)
{
	if (this->grd != 0) return;
	this->grid_height = height;
}

size_t sys::getGridDepth() const
{
	return this->grid_depth;
}

size_t& sys::getGridDepth()
{
	return this->grid_depth;
}

void sys::setGridDepth(size_t depth)
{
	if (this->grd != 0) return;
	this->grid_depth = depth;
}

void sys::setGridSize(size_t width, size_t height, size_t depth)
{
	this->setGridWidth(width);
	this->setGridHeight(height);
	this->setGridDepth(depth);
}

char* sys::askConfig(bool save)
{
	FILE* fhandle;
	char* msg;
	char* buffer;
	char* fname;
	bool exist;

	if ((buffer = reinterpret_cast<char*>(malloc(sizeof(char) * BUFSIZ))) == 0) return 0;
	if ((msg = reinterpret_cast<char*>(malloc(sizeof(char) * BUFSIZ))) == 0)
	{
		free(buffer);
		return 0;
	}
	if ((fname = reinterpret_cast<char*>(malloc(sizeof(char) * BUFSIZ))) == 0)
	{
		free(msg);
		free(buffer);
		return 0;
	}

	if (sprintf(msg, "Configuration file to %s (leave empty for \"default\")", save?"save to":"load from") == 0)
	{
		free(fname);
		free(msg);
		free(buffer);
		return 0;
	}

	buffer = prompt_string(msg, buffer, BUFSIZ); free(msg);
	if (strlen(buffer) == 0)
	{
		printf("No filename entered, assuming default\n");
		strcpy(buffer, "default");
	}

	sprintf(fname, "%s%s", buffer, config_suffix);
	if ((fhandle = fopen(fname, "r")) == 0)
	{
		exist = false;
	}
	else
	{
		exist = true;
		fclose(fhandle);
	}
	free(fname);

	if (save == true && exist == true)
	{
		if (prompt_yn("File already exist, save anyway?") == false)
		{
			printf("Save cancelled\n");
			free(buffer);
			return 0;
		}
	}
	else if (save == false && exist == false)
	{
		printf("ERROR: Configuration file doesn't exist\n");
		free(buffer);
		return 0;
	}
	return buffer;
}

bool sys::loadConfig(const char* _fname)
{
	FILE* fhandle;
	char* fname;
	struct struct_config cfg;

	if ((this->th != 0) || (this->grd != 0)) return false;
	if ((fname = reinterpret_cast<char*>(malloc(sizeof(char) * BUFSIZ))) == 0) return false;
	if (sprintf(fname, "%s%s", _fname, config_suffix) == 0)
	{
		free(fname);
		return false;
	}

	if ((fhandle = fopen(fname, "rb")) == 0)
	{
		free(fname);
		return false;
	}

	if (fread(&cfg, sizeof(cfg), 1, fhandle) != 1)
	{
		fclose(fhandle);
		free(fname);
		return false;
	}
	fclose(fhandle);
	free(fname);

	if (cfg.signature == config_signature)
	{
		this->setRings(cfg.rings);
		this->setRadius(cfg.radius);
		this->setCharges(cfg.charges);
		this->setVoltage(cfg.voltage);
		this->setIterations(cfg.iterations);
		this->setThreads(cfg.threads);

		this->setGridWidth(cfg.grid_width);
		this->setGridHeight(cfg.grid_height);
		this->setGridDepth(cfg.grid_depth);

		return true;
	}
	return false;
}

bool sys::saveConfig(const char* _fname)
{
	FILE* fhandle;
	char* fname;
	struct struct_config cfg;

	if ((fname = reinterpret_cast<char*>(malloc(sizeof(char) * BUFSIZ))) == 0) return false;
	if (sprintf(fname, "%s%s", _fname, config_suffix) == 0)
	{
		free(fname);
		return false;
	}

	cfg.signature = config_signature;

	cfg.rings = this->getRings();
	cfg.radius = this->getRadius();
	cfg.charges = this->getCharges();
	cfg.voltage = this->getVoltage();
	cfg.iterations = this->getIterations();
	cfg.threads = this->getThreads();

	cfg.grid_width = this->getGridWidth();
	cfg.grid_height = this->getGridHeight();
	cfg.grid_depth = this->getGridDepth();

	if ((fhandle = fopen(fname, "wb")) == 0)
	{
		free(fname);
		return false;
	}
	free(fname);

	if (fwrite(&cfg, sizeof(cfg), 1, fhandle) != 1)
	{
		fclose(fhandle);
		return false;
	}
	fclose(fhandle);
	return true;
}

bool sys::defaultConfig()
{
	size_t concurrency;

	if ((this->th != 0) || (this->grd != 0)) return false;
	concurrency = boost::thread::hardware_concurrency();
	if (concurrency < 1) concurrency = 1;

	this->setRings(6);
	this->setRadius(1.00);
	this->setCharges(10);
	this->setVoltage(400);
	this->setIterations(1000000 * concurrency); /* 1000000 */
	this->setThreads(concurrency);

	this->setGridWidth(100); /* 100 */
	this->setGridHeight(100); /* 100 */
	this->setGridDepth(100); /* 100 */

	return true;
}

bool sys::main()
{
	char* buffer;
	char* fname;
	menu conf;
	size_t i;

	if ((buffer = reinterpret_cast<char*>(malloc(sizeof(char) * BUFSIZ))) == 0) return false;
	conf.setHeader("\nEnergy Field Calculation Options\n================================");
	conf.setFooter("--------------------------------");
	conf.setPrompt("Select configuration option");
	this->loadConfig();

	i = 0;
	while (i < 17)
	{
		conf.addOption(0);
		i++;
	}

	while (true)
	{
		i = 0;

		sprintf(buffer, "Run");
		conf.setOption(i++, buffer);
		sprintf(buffer, "Clear last run\n");
		conf.setOption(i++, buffer);

		sprintf(buffer, "Number of rings: %lu", this->getRings());
		conf.setOption(i++, buffer);
		sprintf(buffer, "Ring radius: %lf", this->getRadius());
		conf.setOption(i++, buffer);
		sprintf(buffer, "Charges per ring: %lu", this->getCharges());
		conf.setOption(i++, buffer);
		sprintf(buffer, "Voltage from charges: %lf V", this->getVoltage());
		conf.setOption(i++, buffer);
		sprintf(buffer, "Iterations: %lu (Actual: %lu, %lu per thread, %lu skipped)", this->getIterations(), (this->getIterations() / this->getThreads()) * this->getThreads(), (this->getIterations() / this->getThreads()), this->getIterations() - ((this->getIterations() / this->getThreads()) * this->getThreads()));
		conf.setOption(i++, buffer);
		sprintf(buffer, "CPU Threads: %lu (Max hardware concurrency: %u)\n", this->getThreads(), boost::thread::hardware_concurrency());
		conf.setOption(i++, buffer);

		sprintf(buffer, "Grid width: %lu", this->getGridWidth());
		conf.setOption(i++, buffer);
		sprintf(buffer, "Grid height: %lu", this->getGridHeight());
		conf.setOption(i++, buffer);
		sprintf(buffer, "Grid depth: %lu\n", this->getGridDepth());
		conf.setOption(i++, buffer);

		sprintf(buffer, "Show last run results");
		conf.setOption(i++, buffer);
		sprintf(buffer, "Save last run results\n");
		conf.setOption(i++, buffer);

		sprintf(buffer, "Load configuration");
		conf.setOption(i++, buffer);
		sprintf(buffer, "Save configuration");
		conf.setOption(i++, buffer);
		sprintf(buffer, "Default configuration\n");
		conf.setOption(i++, buffer);

		sprintf(buffer, "Exit program");
		conf.setOption(i++, buffer);

		i = conf.askMenu();
		switch (i)
		{
			case 0: /* Run */
				if ((this->th != 0) || (this->grd != 0))
				{
					printf("ERROR: Please clear the last run before running new one\n");
				}
				else				
				{
					this->run();
				}
				break;
			case 1: /* Clear last run */
				this->clear();
				printf("Last run results cleared\n");
				break;
			case 2: /* Number of rings */
				if ((this->th != 0) || (this->grd != 0))
				{
					printf("ERROR: Please clear last run before setting new options\n");
				}
				else
				{
					this->setRings(prompt_unsignedlong("Set number of rings"));
				}
				break;
			case 3: /* Ring radius */
				if ((this->th != 0) || (this->grd != 0))
				{
					printf("ERROR: Please clear last run before setting new options\n");
				}
				else
				{
					this->setRadius(prompt_double("Set ring radius"));
				}
				break;
			case 4: /* Charges per ring */
				if ((this->th != 0) || (this->grd != 0))
				{
					printf("ERROR: Please clear last run before setting new options\n");
				}
				else
				{
					this->setCharges(prompt_unsignedlong("Set charges per ring"));
				}
				break;
			case 5: /* Voltage */
				if ((this->th != 0) || (this->grd != 0))
				{
					printf("ERROR: Please clear last run before setting new options\n");
				}
				else
				{
					this->setVoltage(prompt_double("Set voltage from charges"));
				}
				break;
			case 6: /* Iterations */
				if ((this->th != 0) || (this->grd != 0))
				{
					printf("ERROR: Please clear last run before setting new options\n");
				}
				else
				{
					this->setIterations(prompt_unsignedlong("Set total number of iterations"));
				}
				break;
			case 7: /* CPU Threads */
				if ((this->th != 0) || (this->grd != 0))
				{
					printf("ERROR: Please clear last run before setting new options\n");
				}
				else
				{
					this->setThreads(prompt_unsignedlong("Set number of threads"));
				}
				break;
			case 8: /* Grid width */
				if ((this->th != 0) || (this->grd != 0))
				{
					printf("ERROR: Please clear last run before setting new options\n");
				}
				else
				{
					this->setGridWidth(prompt_unsignedlong("Set grid width"));
				}
				break;
			case 9: /* Grid height */
				if ((this->th != 0) || (this->grd != 0))
				{
					printf("ERROR: Please clear last run before setting new options\n");
				}
				else
				{
					this->setGridHeight(prompt_unsignedlong("Set grid height"));
				}
				break;
			case 10: /* Grid depth */
				if ((this->th != 0) || (this->grd != 0))
				{
					printf("ERROR: Please clear last run before setting new options\n");
				}
				else
				{
					this->setGridDepth(prompt_unsignedlong("Set grid depth"));
				}
				break;
			case 11: /* Show last run results */
				if ((this->th != 0) || (this->grd != 0))
				{
					this->runResults();
				}
				else
				{
					printf("ERROR: Please make a run before using this option\n");
				}
				break;
			case 12: /* Save last run results */
				if ((this->th != 0) || (this->grd != 0))
				{
					this->saveResults();
				}
				else
				{
					printf("ERROR: Please make a run before using this option\n");
				}
				break;
			case 13: /* Load configuration */
				if ((this->th != 0) || (this->grd != 0))
				{
					printf("ERROR: Please clear last run before setting new options\n");
				}
				else
				{
					if ((fname = this->askConfig(false)) != 0)
					{
						if (this->loadConfig(fname) == true)
						{
							printf("Configuration loaded successfully\n");
						}
						else
						{
							printf("ERROR: Failed to load configuration\n");
						}
						free(fname);
					}
				}
				break;
			case 14: /* Save configuration */
				if ((fname = this->askConfig(true)) != 0)
				{
					if (this->saveConfig(fname) == true)
					{
						printf("Configuration saved successfully\n");
					}
					else
					{
						printf("ERROR: Failed to save configuration\n");
					}
					free(fname);
				}
				break;
			case 15: /* Default configuration */
				if ((this->th != 0) || (this->grd != 0))
				{
					printf("ERROR: Please clear last run before setting new options\n");
				}
				else
				{
					if (this->defaultConfig() == true) printf("Default configuration loaded successfully\n");
				}
				break;
			case 16: /* Exit program */
				free(buffer);
				return true;
				break;
			default: /* ERROR */
				printf("Unexpected error\n");
				free(buffer);
				return false;
				break;
		}
	}
}

bool sys::run()
{
	bool errors;

	errors = false;
	if (this->getRings() == 0)
	{
		printf("ERROR: No rings\n");
		errors = true;
	}

	if (this->getCharges() == 0)
	{
		printf("ERROR: No charges per ring\n");
		errors = true;
	}

	if (this->getIterations() == 0)
	{
		printf("ERROR: No iterations");
		errors = true;
	}

	if (this->getThreads() == 0)
	{
		printf("ERROR: No threads\n");
		errors = true;
	}

	if (this->getGridWidth() == 0)
	{
		printf("ERROR: Invalid grid width\n");
		errors = true;
	}

	if (this->getGridHeight() == 0)
	{
		printf("ERROR: Invalid grid height\n");
		errors = true;
	}

	if (this->getGridDepth() == 0)
	{
		printf("ERROR: Invalid grid depth\n");
		errors = true;
	}

	if (this->th != 0)
	{
		printf("ERROR: System has already run\n");
		errors = true;
	}

	if (this->grd != 0)
	{
		printf("ERROR: System has already run\n");
		errors = true;
	}

	if ((this->getIterations() / this->getThreads()) == 0)
	{
		printf("ERROR: Number of iterations too low\n");
		errors = true;
	}

	if (errors == true) return false;
	this->runIterations();
	this->runLowestDistrib();
	this->runGrid();
	this->runResults();

	/* TODO: sys::run() */
	return false;
}

const rings_thread*const* sys::getRingsArray() const
{
	return this->th;
}

rings_thread** sys::getRingsArray()
{
	return this->th;
}

const grid* sys::getGrid() const
{
	return this->grd;
}

grid* sys::getGrid()
{
	return this->grd;
}

