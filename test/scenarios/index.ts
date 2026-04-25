import { BaseScenario } from './BaseScenario';
import { WhereClauseScenario } from './WhereClauseScenario';

export const scenarios: Record<string, BaseScenario> = {
    'where-clause': new WhereClauseScenario(),
};
